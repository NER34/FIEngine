#pragma once
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"

class Vector3Parameter : public MaterialParameterBase<Vector3Parameter>
{
public:
    
    Vector3Parameter() = delete;
    Vector3Parameter(const std::string& name, GLuint shaderProgramID, const glm::vec3& matrix);
    
    void ApplyParameter() const override;
    
private:
    
    GLint m_location = -1;
    glm::vec3 m_vector;
    
};
