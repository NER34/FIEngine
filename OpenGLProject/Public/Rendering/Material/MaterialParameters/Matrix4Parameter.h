#pragma once
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"

class Matrix4Parameter : public MaterialParameterBase<Matrix4Parameter>
{
public:
    
    Matrix4Parameter() {}
    Matrix4Parameter(const std::string& name, const glm::mat4& matrix)
        : m_name(name), m_matrix(matrix)
    {}
    
    void ApplyParameter(ShaderProgram* shaderProgram) const override;
    
private:
    
    std::string m_name;
    glm::mat4 m_matrix;
    
};
