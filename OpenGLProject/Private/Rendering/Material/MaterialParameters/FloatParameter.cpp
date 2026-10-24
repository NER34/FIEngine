#include "Rendering/Material/MaterialParameters/FloatParameter.h"

#include "Rendering/Material/ShaderProgram.h"

void FloatParameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    shaderProgram->SetUniformFloat(m_name, m_value);
}
