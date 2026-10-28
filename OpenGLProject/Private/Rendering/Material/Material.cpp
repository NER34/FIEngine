#include "Rendering/Material/Material.h"

Material::~Material()
{
    ClearMaterial();
}

void Material::SetShaderProgram(const ResourceHandle<ShaderProgramResource>& shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

const ResourceHandle<ShaderProgramResource>& Material::GetShaderProgram() const
{
    return m_shaderProgram;
}

void Material::Activate() const
{
    if (IsValid())
    {
        m_shaderProgram->Activate();
        
        for (const auto& parameter : m_parameters.GetParameters() | std::views::values)
        {
            parameter->ApplyParameter(m_shaderProgram);
        }
    }
    else
    {
        LOG_ERROR("Failed activate material: shader program is invalid");
    }
}

void Material::Deactivate() const
{
    if (m_shaderProgram.IsValid())
    {
        m_shaderProgram->Deactivate();
    }
}

GLuint Material::GetShaderProgramID() const
{
    return m_shaderProgram.IsValid() ? m_shaderProgram->GetShaderProgramID() : 0;
}

bool Material::IsValid() const
{
    return m_shaderProgram.IsValid() && m_shaderProgram->IsValid();
}

void Material::ClearMaterial()
{
    m_shaderProgram = ResourceHandle<ShaderProgramResource>();
    m_parameters.ClearParameters();
}
