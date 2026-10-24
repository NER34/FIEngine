#include "Rendering/Renderer.h"

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
    
    glm::mat4 projMat = m_camera.getProjectionMatrix();
    glm::mat4 viewMat = m_camera.getViewMatrix();
    glm::mat4 viewProjMat = projMat * viewMat * toLeftHandTransformMat;
    
    for (const auto& drawDesc : m_frontDrawList)
    {
        glm::mat4 modelMat = drawDesc.m_transform.GetModelMatrix();
        auto* shaderProgram = drawDesc.m_material.GetShaderProgram();
        
        drawDesc.m_material.activate();
        
        shaderProgram->SetUniformMat4("projViewMat", viewProjMat);
        shaderProgram->SetUniformMat4("modelMat", modelMat);
        shaderProgram->SetUniformFloat3("lightPos", m_lightPoint.getPosition());
        shaderProgram->SetUniformFloat3("lightColor", m_lightPoint.getColor());
        shaderProgram->SetUniformFloat3("viewPos", m_camera.getPosition());
        
        drawDesc.m_mesh.draw();
        
        drawDesc.m_material.deactivate();
    }
}

Camera& Renderer::getCamera()
{
    return m_camera;
}

LightPoint& Renderer::getLightPoint()
{
    return m_lightPoint; 
}

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
