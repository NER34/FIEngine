#include "Rendering/Material/Material.h"

/*
Material::Material(const MaterialDesc& desc)
{
    //m_shaderProgram = desc.m_shaderProgram;
    //m_textures = desc.m_texturesDesc;
    //setupTextureSamplers(desc);
}
*/

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

/*
void Material::setup(const MaterialDesc& desc)
{
    clearMaterial();
    
    m_shaderProgram = desc.m_shaderProgram;
    m_textures = desc.m_texturesDesc;
    setupTextureSamplers(desc);
}
*/

void Material::activate() const
{
    if (isValid())
    {
        m_shaderProgram->activate();
        
        for (IMaterialParameter* parameter : m_parametersPool)
        {
            parameter->ApplyParameter();
        }
        
        /*
        GLuint textureSlot = GL_TEXTURE0;
        for (uint32_t textureIndex = 0; textureIndex < m_textures.size(); textureIndex++, textureSlot++)
        {
            const Texture* texture = m_textures[textureIndex].m_texture;
            if (texture != nullptr)
            {
                texture->activate(textureSlot);
            }
        }
        */
        
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

/*
void Material::setupTextureSamplers(const MaterialDesc& desc)
{
    if (!isValid())
    {
        return;
    }
    
    m_shaderProgram->activate();
    
    for (uint32_t i = 0; i < desc.m_texturesDesc.size(); i++)
    {
        const auto& samplerName = desc.m_texturesDesc[i].m_name;
        GLint uniformLoc = glGetUniformLocation(getShaderProgramID(), samplerName.c_str());
        CHECK_OGL_ERR(glUniform1i(uniformLoc, i))
    }
    
    m_shaderProgram->deactivate();
}
*/

void Material::clearMaterial()
{
    m_shaderProgram = nullptr;
    m_parametersPool.ClearParameters();
}
