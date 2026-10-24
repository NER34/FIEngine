#pragma once
#include <string>

#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"

class Vector3Parameter : public MaterialParameterBase<Vector3Parameter>
{
public:
    
    Vector3Parameter() {}
    Vector3Parameter(const std::string& name, const glm::vec3& vec)
        : m_name(name), m_vector(vec)
    {}
    
    void ApplyParameter(ShaderProgram* shaderProgram) const override;
    
private:
    
    std::string m_name;
    glm::vec3 m_vector;
    
};
