#include "Rendering/Material/Material.h"

Material::~Material()
{
    clearMaterial();
}

void Material::SetShaderProgram(const ShaderProgram* shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

void Material::activate() const
{
    if (isValid())
    {
        m_shaderProgram->activate();
        
        for (const IMaterialParameter& parameter : m_parameters)
        {
            parameter.ApplyParameter();
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
        m_shaderProgram->deactivate();
    }
}

GLuint Material::getShaderProgramID() const
{
    return m_shaderProgram ? m_shaderProgram->getShaderProgramID() : 0;
}

bool Material::isValid() const
{
    return m_shaderProgram != nullptr && m_shaderProgram->isValid();
}

void Material::clearMaterial()
{
    m_shaderProgram = nullptr;
    m_parameters.ClearParameters();
}
