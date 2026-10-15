#include "Rendering/Material/MaterialParameters/Matrix4Parameter.h"

#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Validation.h"

Matrix4Parameter::Matrix4Parameter(
    const std::string& name, GLuint shaderProgramID, const glm::mat4& matrix) 
    : m_matrix(matrix)
{
    CHECK_OGL_ERR(m_location = glGetUniformLocation(shaderProgramID, name.c_str()))
}

void Matrix4Parameter::ApplyParameter()
{
    CHECK_OGL_ERR(glUniformMatrix4fv(m_location, 1, false, glm::value_ptr(m_matrix)))
}
