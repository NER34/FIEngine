#include "Rendering/Material/MaterialParameters/Matrix4Parameter.h"

#include "Rendering/Material/ShaderProgram.h"

void Matrix4Parameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    shaderProgram->SetUniformMat4(m_name, m_matrix);
}
