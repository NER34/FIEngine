#pragma once
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"

class Matrix4Parameter : public MaterialParameterBase<Matrix4Parameter>
{
public:
    
    Matrix4Parameter() = delete;
    Matrix4Parameter(const std::string& name, GLuint shaderProgramID, const glm::mat4& matrix);
    
    void ApplyParameter() override;
    
private:
    
    glm::mat4 m_matrix;
    GLint m_location = -1;
    
};
