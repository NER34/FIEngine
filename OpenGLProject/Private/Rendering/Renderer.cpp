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
    glClear(GL_COLOR_BUFFER_BIT);
    
    glm::mat4 projMat = m_camera.getProjectionMatrix();
    glm::mat4 viewMat = m_camera.getViewMatrix();
    glm::mat4 viewProjMat = projMat * viewMat * toLeftHandTransformMat;
    
    for (const auto& drawDesc : m_frontDrawList)
    {
        glm::mat4 modelMat = drawDesc.m_transform.getModelMatrix();
        
        drawDesc.m_material.activate();
        drawDesc.m_material.setUniform("mvp", viewProjMat * modelMat);
        
        drawDesc.m_mesh.draw();
        
        drawDesc.m_material.deactivate();
    }
}

Camera& Renderer::getCamera()
{
    return m_camera;
}

void Renderer::initialize(const RendererDesc& desc)
{
	gladLoadGL();
    
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
    
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
