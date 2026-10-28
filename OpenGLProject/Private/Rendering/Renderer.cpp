#include "Rendering/Renderer.h"

#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"
#include "Components/TransformComponent.h"
#include "Scene/SceneManager.h"

Renderer& Renderer::get()
{
    static Renderer instance;
    return instance;
}

void Renderer::render()
{
    std::swap(m_frontDrawList, m_backDrawList);
    m_backDrawList.clear();
    
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
    glm::mat4 projMat = camera.GetProjectionMatrix(m_camera.getCameraDesc().m_aspectRatio);
    glm::mat4 viewMat = camera.GetViewMatrix(cameraTransform.m_transform);
    glm::mat4 viewProjMat = projMat * viewMat * toLeftHandTransformMat;
    
    // Light
    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(0.0f, 0.0f, 0.0f);
    auto lightView = SceneManager::Get().View<LightComponent, TransformComponent>();
    auto lightEntity = lightView.begin();                                                    
    if (lightEntity != lightView.end())                                                      
    {                                                                          
        const auto& [light, lightTransform] = lightView.get<LightComponent, TransformComponent>(*lightEntity);
        lightPos = lightTransform.m_transform.m_position;
        lightColor = light.m_lightColor;
    }
    
    for (const auto& drawDesc : m_frontDrawList)
    {
        glm::mat4 modelMat = drawDesc.m_transform.GetModelMatrix();
        const auto* mesh = drawDesc.m_mesh.Get();
        for (size_t i = 0; i < mesh->GetSubMeshCount(); i++)
        {
            auto* material = drawDesc.m_material.Get()->GetMaterial();
            auto* shaderProgram = material->GetShaderProgram();
            
            material->activate();
            
            shaderProgram->SetUniformMat4("projViewMat", viewProjMat);
            shaderProgram->SetUniformMat4("modelMat", modelMat);
            shaderProgram->SetUniformFloat3("lightPos", lightPos);
            shaderProgram->SetUniformFloat3("lightColor", lightColor);
            shaderProgram->SetUniformFloat3("viewPos", cameraTransform.m_transform.m_position);
            
            mesh->DrawSubMesh(i);
            
            material->deactivate();
        }
    }
}

/*
Camera& Renderer::getCamera()
{
    return m_camera;
}

LightPoint& Renderer::getLightPoint()
{
    return m_lightPoint; 
}
*/

void Renderer::initialize(const RendererDesc& desc)
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
    
    m_camera = Camera(desc.m_cameraDesc);
}

void Renderer::addDraw(const GeometryDrawDesc& desc)
{
    m_backDrawList.push_back(desc);
}
