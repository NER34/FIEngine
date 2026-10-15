#include "Rendering/Material/Material.h"

/*
Material::Material(Material&& other) noexcept
{
    m_shaderProgram = other.m_shaderProgram;
    m_paramsBuffer = std::move(other.m_paramsBuffer);
    m_currBufferOffset = other.m_currBufferOffset;
    
    other.m_shaderProgram = nullptr;
    other.m_currBufferOffset = 0;
}

Material& Material::operator=(Material&& other) noexcept
{
    if (this != &other)
    {
        m_shaderProgram = other.m_shaderProgram;
        m_paramsBuffer = std::move(other.m_paramsBuffer);
        m_currBufferOffset = other.m_currBufferOffset;
    
        other.m_shaderProgram = nullptr;
        other.m_currBufferOffset = 0;
    }
    return *this;
}
*/

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
        
        for (const IMaterialParameter* parameter : m_parametersPool)
        {
            parameter->ApplyParameter();
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
    m_parametersPool.ClearParameters();
}
