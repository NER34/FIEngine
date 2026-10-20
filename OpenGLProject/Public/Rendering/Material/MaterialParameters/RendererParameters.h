#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material/MaterialParameter.h"

class Mesh;

class RendererParameters : public MaterialParameterBase<RendererParameters>
{
public:
    
    RendererParameters() {};
    RendererParameters(GLuint shaderProgramID);
    
    void SetTransform(const Transform& transform);
    
    void ApplyParameter() const override;

private:
    
    Transform m_transform;
    GLint m_projViewMatLoc = -1;
    GLint m_modelMatLoc = -1;
    GLint m_lightPosLoc = -1;
    GLint m_lightColorLoc = -1;
    GLint m_viewPosLoc = -1;
    
};
