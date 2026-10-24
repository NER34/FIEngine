#include "Rendering/Material/Material.h"

Material::~Material()
{
    clearMaterial();
}

void Material::SetShaderProgram(ShaderProgram* shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

ShaderProgram* Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

void Material::activate() const
{
    if (isValid())
    {
        m_shaderProgram->Activate();
        
        for (const IMaterialParameter& parameter : m_parameters)
        {
            parameter.ApplyParameter(m_shaderProgram);
        }
    }
    else
    {
        LOG_ERROR("Failed activate material: shader program is invalid");
    }
}

void Material::deactivate() const
{
    if (m_shaderProgram)
    {
        m_shaderProgram->Deactivate();
    }
}

GLuint Material::getShaderProgramID() const
{
    return m_shaderProgram ? m_shaderProgram->GetShaderProgramID() : 0;
}

bool Material::isValid() const
{
    return m_shaderProgram != nullptr && m_shaderProgram->IsValid();
}

void Material::clearMaterial()
{
    m_shaderProgram = nullptr;
    m_parameters.ClearParameters();
}
