#include "Rendering/Material/MaterialParameters/Vector3Parameter.h"

#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Validation.h"

Vector3Parameter::Vector3Parameter(
    const std::string& name, GLuint shaderProgramID, const glm::vec3& matrix) 
    : m_vector(matrix)
{
    CHECK_OGL_ERR(m_location = glGetUniformLocation(shaderProgramID, name.c_str()))
}

void Vector3Parameter::ApplyParameter()
{
    CHECK_OGL_ERR(glUniform3fv(m_location, 1, glm::value_ptr(m_vector)))
}
