#include "Rendering/Material/MaterialParameters/RendererParameters.h"

#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Renderer.h"
#include "Rendering/Validation.h"

RendererParameters::RendererParameters(GLuint shaderProgramID)
{
    CHECK_OGL_ERR(m_projViewMatLoc  = glGetUniformLocation(shaderProgramID, "projViewMat"))
    CHECK_OGL_ERR(m_modelMatLoc     = glGetUniformLocation(shaderProgramID, "modelMat"))
    CHECK_OGL_ERR(m_lightPosLoc     = glGetUniformLocation(shaderProgramID, "lightPos"))
    CHECK_OGL_ERR(m_lightColorLoc   = glGetUniformLocation(shaderProgramID, "lightColor"))
    CHECK_OGL_ERR(m_viewPosLoc      = glGetUniformLocation(shaderProgramID, "viewPos"))
}

void RendererParameters::SetTransform(const Transform& transform)
{
    m_transform = transform;
}

void RendererParameters::ApplyParameter() const
{
    Renderer& renderer = Renderer::get();
    glm::mat4 projMat = renderer.getCamera().getProjectionMatrix();
    glm::mat4 viewMat = renderer.getCamera().getViewMatrix();
    glm::mat4 viewProjMat = projMat * viewMat * toLeftHandTransformMat;
    CHECK_OGL_ERR(glUniformMatrix4fv(
        m_modelMatLoc, 1, false, 
        glm::value_ptr(m_transform.getModelMatrix())
        ))
    CHECK_OGL_ERR(glUniformMatrix4fv(
        m_projViewMatLoc, 1, false, 
        glm::value_ptr(viewProjMat)
        ))
    CHECK_OGL_ERR(glUniform3fv(
        m_lightPosLoc, 1, 
        glm::value_ptr(renderer.getLightPoint().getPosition())
        ))
    CHECK_OGL_ERR(glUniform3fv(
        m_lightColorLoc, 1, 
        glm::value_ptr(renderer.getLightPoint().getColor())
        ))
    CHECK_OGL_ERR(glUniform3fv(
        m_viewPosLoc, 1, 
        glm::value_ptr(renderer.getCamera().getPosition())
        ))
}
