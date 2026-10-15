#include "Rendering/Material/MaterialParameters/FloatParameter.h"

#include "Rendering/Validation.h"

FloatParameter::FloatParameter(
    const std::string& name, GLuint shaderProgramID, float value) 
    : m_value(value)
{
    CHECK_OGL_ERR(m_location = glGetUniformLocation(shaderProgramID, name.c_str()))
}

void FloatParameter::ApplyParameter() const
{
    CHECK_OGL_ERR(glUniform1fv(m_location, 1, &m_value))
}
