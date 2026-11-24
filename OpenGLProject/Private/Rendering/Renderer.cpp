#include "Rendering/Renderer.h"

#include "AssetManager/AssetManager.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Scene/SceneManager.h"
#include "Window/WindowManager.h"
#include "easy/profiler.h"

Renderer& Renderer::Get()
{
    auto* system = SystemLocator::Get().GetSystem<Renderer>();
    assert(system && "Renderer is not registered");
    return *static_cast<Renderer*>(system);
}

void Renderer::Initialize()
{
    EASY_FUNCTION();

    auto* system = SystemLocator::Get().GetSystem<FIE::WindowManager>();
    if (!system)
    {
        LOG_ERROR("Renderer: Failed to get WindowManager");
        return;
    }
    
    auto& windowManager = *static_cast<FIE::WindowManager*>(system);
    const auto& window = windowManager.GetWindow();
    
    //CameraDesc cameraDesc;
    //cameraDesc.m_aspectRatio = (float)window->getWindowDesc().m_width / (float)window->getWindowDesc().m_height;
	
    RendererDesc rendererDesc;
    rendererDesc.m_viewportDesc = {
        0, 0,
        (GLsizei)window->getWindowDesc().m_width,
        (GLsizei)window->getWindowDesc().m_height
    };
    //rendererDesc.m_cameraDesc = cameraDesc;
	
    Initialize(rendererDesc);
}

void Renderer::Update(float deltaTime)
{
    EASY_FUNCTION();

    Render();
}

void Renderer::Deinitialize()
{
    EASY_FUNCTION();

    m_defaultMaterial.Reset();
}

void Renderer::Render()
{
    CollectMeshesForDrawing();
    
    std::swap(m_frontDrawList, m_backDrawList);
    m_backDrawList.clear();
    
    auto* system = SystemLocator::Get().GetSystem<FIE::WindowManager>();
    if (!system)
    {
        LOG_ERROR("Renderer: Failed to get WindowManager");
        return;
    }
    auto& windowManager = *static_cast<FIE::WindowManager*>(system);
    const auto& window = windowManager.GetWindow();
    float aspectRatio = (float)window->getWindowDesc().m_width / (float)window->getWindowDesc().m_height;
    
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
    
    for (const auto& drawDesc : m_frontDrawList)
    {
        const glm::mat4& modelMat = drawDesc.m_transform;
        const auto& mesh = drawDesc.m_mesh;

        const auto& material = drawDesc.m_material;
        if (!material.IsValid())
        {
            continue;
        }
        
        material->Activate();
        
        const auto& shaderProgram = material->GetShaderProgram();
        shaderProgram->SetUniformMat4("projViewMat", viewProjMat);
        shaderProgram->SetUniformMat4("modelMat", modelMat);
        shaderProgram->SetUniformFloat3("lightPos", lightPos);
        shaderProgram->SetUniformFloat3("lightColor", lightColor);
        shaderProgram->SetUniformFloat3("viewPos", cameraTransform.GetTransform().m_position);
        
        mesh->DrawSubMesh(drawDesc.m_subMeshIndex);
        
        material->Deactivate();
    }
}

void Renderer::CollectMeshesForDrawing()
{    
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

void Renderer::Initialize(const RendererDesc& desc)
{
	gladLoadGL();
    
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    glViewport(
        desc.m_viewportDesc.x,
        desc.m_viewportDesc.y,
        desc.m_viewportDesc.width,
        desc.m_viewportDesc.height
        );
    
    //m_camera = Camera(desc.m_cameraDesc);
    
#define DEFAULT_MATERIAL_PATH "Materials/DefaultGrid.asset"
    if (!AssetManager::Get().Load<MaterialResource>(DEFAULT_MATERIAL_PATH, m_defaultMaterial))
    {
        LOG_ERROR("Renderer: Failed to load {} material", DEFAULT_MATERIAL_PATH);
    }
}

void Renderer::AddDraw(const GeometryDrawDesc& desc)
{
    m_backDrawList.push_back(desc);
}
