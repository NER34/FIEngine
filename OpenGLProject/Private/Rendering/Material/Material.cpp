#include "Rendering/Material/Material.h"

#include <ranges>

#include "Rendering/Validation.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/TextureResource.h"

Material::Material(const MaterialDesc& desc)
{
    createShaders(desc);
    createShaderProgram(desc);
    createTextures(desc);
    setupTextureSamplers(desc);
}

Material::Material(Material&& other) noexcept
{  
    m_shaderProgramID = other.m_shaderProgramID;
    other.m_shaderProgramID = 0;
    
    m_shaders.swap(other.m_shaders);
    other.m_shaders.clear();
}

Material& Material::operator=(Material&& other) noexcept
{
    if (this != &other)
    {
        m_shaderProgramID = other.m_shaderProgramID;
        m_shaderProgramID = 0;
        
        m_shaders.swap(other.m_shaders);
        other.m_shaders.clear();
    }
    return *this;
}

Material::~Material()
{
    destroyMaterial();
}

void Material::setup(const MaterialDesc& desc)
{
    destroyMaterial();
    createShaders(desc);
    createShaderProgram(desc);
    createTextures(desc);
    setupTextureSamplers(desc);
}

void Material::activate() const
{
    if (isValid())
    {
        glUseProgram(m_shaderProgramID);
        GLuint textureSlot = GL_TEXTURE0;
        uint32_t textureIndex = 0;
        for (; textureIndex < m_textures.size(); textureIndex++, textureSlot++)
        {
            glActiveTexture(textureSlot);
            const Texture* texture = m_textures[textureIndex].Get()->GetTexture();
            glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
        }
        //CHECK_OGL_ERR("Material glUseProgram")
    }
    else
    {
        LOG_ERROR("Failed activate material: m_shaderProgramID is invalid");
    }
}

void Material::deactivate() const
{
    glUseProgram(0);
}

GLuint Material::getShaderProgramID() const
{
    return m_shaderProgramID;
}

bool Material::isValid() const
{
    return m_shaderProgramID != 0;
}

void Material::createShaderProgram(const MaterialDesc& desc)
{
    m_shaderProgramID = glCreateProgram();
    for (const Shader& shader : m_shaders)
    {
        CHECK_OGL_ERR(glAttachShader(m_shaderProgramID, shader.getShaderID()))
    }
    CHECK_OGL_ERR(glLinkProgram(m_shaderProgramID))

    GLint linkStatus = 0;
    CHECK_OGL_ERR(glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus))

    if (linkStatus == GL_FALSE)
    {
        GLchar infoLog[1024] = {};
        CHECK_OGL_ERR(glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog))
        LOG_ERROR("Failed to link shader program: {}", infoLog);
    }
}

void Material::destroyMaterial()
{
    if (isValid())
    {
        CHECK_OGL_ERR(glDeleteProgram(m_shaderProgramID))
        m_shaderProgramID = 0;
        m_shaders.clear();
        m_textures.clear();
    }
}

void Material::createShaders(const MaterialDesc& desc)
{
    m_shaders.insert(
        m_shaders.begin(), 
        desc.m_shadersDesc.begin(), 
        desc.m_shadersDesc.end()
        );
}

void Material::createTextures(const MaterialDesc& desc)
{
    for (auto& textureLoadDesc : desc.m_texturesDesc)
    {
        m_textures.push_back(ResourceManager::Get().LoadResource<TextureResource>(
            /*textureLoadDesc.m_name, */textureLoadDesc.m_path
            ));
    }
}

void Material::setupTextureSamplers(const MaterialDesc& desc)
{
    glUseProgram(m_shaderProgramID);
    
    for (uint32_t i = 0; i < desc.m_texturesDesc.size(); i++)
    {
        const auto& samplerName = desc.m_texturesDesc[i].m_name;
        GLint uniformLoc = glGetUniformLocation(m_shaderProgramID, samplerName.c_str());
        glUniform1i(uniformLoc, i);
    }
    
    glUseProgram(0);
}
