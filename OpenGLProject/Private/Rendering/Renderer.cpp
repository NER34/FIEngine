#include "Rendering/Renderer.h"

#include "AssetManager/AssetManager.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Scene/SceneManager.h"
#include "Window/WindowManager.h"
#include "easy/profiler.h"
#include "Rendering/Validation.h"

Renderer& Renderer::Get()
{
    auto* system = SystemLocator::Get().GetSystem<Renderer>();
    assert(system && "Renderer is not registered");
    return *static_cast<Renderer*>(system);
}

void Renderer::Initialize()
{
    EASY_FUNCTION()

    auto* system = SystemLocator::Get().GetSystem<FIE::WindowManager>();
    if (!system)
    {
        LOG_ERROR("Renderer: Failed to get WindowManager");
        return;
    }
    
    m_windowManager = static_cast<FIE::WindowManager*>(system);
    const auto& window = m_windowManager->GetWindow();
    m_framebufferResizeHandle = window->m_windowCallbacks.m_onFramebufferResized.BindMemberFunction(
        this, &Renderer::OnFramebufferResize);

    RendererDesc rendererDesc;
    rendererDesc.m_viewportDesc = {
        0, 0,
        (GLsizei)window->getFramebufferWidth(),
        (GLsizei)window->getFramebufferHeight()
    };
	
    Initialize(rendererDesc);
}

void Renderer::Update(float deltaTime)
{
    EASY_FUNCTION()

    Render();
}

void Renderer::Deinitialize()
{
    EASY_FUNCTION()

    if (m_windowManager && m_windowManager->GetWindow())
    {
        m_windowManager->GetWindow()->m_windowCallbacks.m_onFramebufferResized.Unbind(m_framebufferResizeHandle);
    }
    m_framebufferResizeHandle.Reset();
    m_windowManager = nullptr;

    m_defaultMaterial.Reset();
    m_drawList_1.clear();
    m_drawList_2.clear();
}

void Renderer::Render()
{
    if (!m_windowManager || !m_windowManager->GetWindow())
    {
        LOG_ERROR("Renderer: No window to render into");
        return;
    }
    const auto& window = m_windowManager->GetWindow();
    if (window->isMinimized())
    {
        return; // 0x0 framebuffer, nothing to draw
    }

    CollectMeshesForDrawing();

    std::swap(m_frontDrawList, m_backDrawList);
    m_backDrawList.clear();

    SortMeshesForDrawing();

    float aspectRatio = window->GetAspectRatio();
    
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Camera
    auto cameraView = SceneManager::Get().View<CameraComponent, TransformComponent>();
    auto cameraEntity = cameraView.begin();                                                    
    if (cameraEntity == cameraView.end())                                                      
    {                                                                          
        return; // no camera in the scene, nothing to render                   
    }   
    const auto& [camera, cameraTransform] = cameraView.get<CameraComponent, TransformComponent>(*cameraEntity);
    glm::mat4 projMat = camera.GetProjectionMatrix(aspectRatio);
    glm::mat4 viewMat = camera.GetViewMatrix(cameraTransform.GetTransform());
    glm::mat4 viewProjMat = projMat * viewMat * toLeftHandTransformMat;
    
    // Light
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(0.0f, 0.0f, 0.0f);
    auto lightView = SceneManager::Get().View<LightComponent, TransformComponent>();
    auto lightEntity = lightView.begin();                                                    
    if (lightEntity != lightView.end())                                                      
    {                                                                          
        const auto& [light, lightTransform] = lightView.get<LightComponent, TransformComponent>(*lightEntity);
        lightPos = lightTransform.GetTransform().m_position;
        lightColor = light.m_lightColor;
    }
    
    EASY_BLOCK("Renderer::Render Drawing")
    
    ResourceHandle<MaterialResource> currMaterial;
    for (const auto& drawDesc : m_frontDrawList)
    {
        const glm::mat4& modelMat = drawDesc.m_transform;
        const auto& mesh = drawDesc.m_mesh;

        const auto& material = drawDesc.m_material;
        if (!material.IsValid())
        {
            continue;
        }
        if (currMaterial != material)
        {
            if (currMaterial.IsValid())
            {
                currMaterial->Deactivate();
            }
            currMaterial = material;
            currMaterial->Activate();
            
            const auto& shaderProgram = material->GetShaderProgram();
            shaderProgram->SetUniformMat4("projViewMat", viewProjMat);
            shaderProgram->SetUniformFloat3("lightPos", lightPos);
            shaderProgram->SetUniformFloat3("lightColor", lightColor);
            shaderProgram->SetUniformFloat3("viewPos", cameraTransform.GetTransform().m_position);
        }
        
        currMaterial->GetShaderProgram()->SetUniformMat4("modelMat", modelMat);
        mesh->DrawSubMesh(drawDesc.m_subMeshIndex);
    }
    if (currMaterial.IsValid())
    {
        currMaterial->Deactivate();
    }
}

void Renderer::OnFramebufferResize(uint32_t width, uint32_t height)
{
    CHECK_OGL_ERR(glViewport(0, 0, (GLsizei)width, (GLsizei)height))
}

void Renderer::CollectMeshesForDrawing()
{    
    EASY_FUNCTION()
    
    SceneManager::Get().View<TransformComponent, MeshComponent>().each(
        [&](entt::entity entity, const TransformComponent& transform, const MeshComponent& meshComponent)
        {
            if (!meshComponent.m_meshHandle.IsValid())
            {
                return;
            }
            
            for (size_t i = 0; i < meshComponent.m_meshHandle->GetSubMeshCount(); i++)
            {
                const ResourceHandle<MaterialResource>* material = &m_defaultMaterial;
                if (i < meshComponent.m_materialHandles.size() && meshComponent.m_materialHandles[i].IsValid())
                {
                    material = &meshComponent.m_materialHandles[i];
                }
                    
                GeometryDrawDesc desc = {
                    meshComponent.m_meshHandle,
                    *material,
                    transform.GetGlobalTransform(),
                    i
                };
                
                AddDraw(desc);
            }
            
        });
}

void Renderer::SortMeshesForDrawing()
{
    EASY_FUNCTION()
    
    std::ranges::sort(m_frontDrawList, [](const GeometryDrawDesc& A, const GeometryDrawDesc& B)
    {
        if (A.m_material < B.m_material)
        {
            return true;
        }
        if (A.m_material == B.m_material)
        {
            if (A.m_mesh < B.m_mesh)
            {
                return true;
            }
            if (A.m_mesh == B.m_mesh)
            {
                return A.m_subMeshIndex < B.m_subMeshIndex;
            }
        }
        return false;
    });
}

void Renderer::Initialize(const RendererDesc& desc)
{
	CHECK_OGL_ERR(gladLoadGL())
    
	//glEnable(GL_CULL_FACE);
	CHECK_OGL_ERR(glDisable(GL_CULL_FACE))
    CHECK_OGL_ERR(glEnable(GL_DEPTH_TEST))
    
    CHECK_OGL_ERR(glViewport(
        desc.m_viewportDesc.x,
        desc.m_viewportDesc.y,
        desc.m_viewportDesc.width,
        desc.m_viewportDesc.height
        ))
    
    
    
    constexpr const char* DEFAULT_MATERIAL_PATH = "Materials/DefaultGrid.asset";
    if (!AssetManager::Get().Load<MaterialResource>(DEFAULT_MATERIAL_PATH, m_defaultMaterial))
    {
        LOG_ERROR("Renderer: Failed to load {} material", DEFAULT_MATERIAL_PATH);
    }
}

void Renderer::AddDraw(const GeometryDrawDesc& desc)
{
    m_backDrawList.push_back(desc);
}
