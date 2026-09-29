#include "Rendering/Texture.h"

#include <cassert>

#include "stb_image.h"

#include "Rendering/Validation.h"

Texture::Texture(const TextureDesc& desc)
{
    createTexture(desc);
}

Texture::Texture(Texture&& other) noexcept
{
    m_textureID = other.m_textureID;
    other.m_textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    if (this != &other)
    {
        m_textureID = other.m_textureID;
        other.m_textureID = 0;
    }
    return *this;
}

Texture::~Texture()
{
    destroyTexture();
}

void Texture::setup(const TextureDesc& desc)
{
    destroyTexture();
    createTexture(desc);
}

void Texture::activate(GLenum textureUnit) const
{
    if (isValid())
    {
        glActiveTexture(textureUnit);
        CHECK_OGL_ERR("Texture glActiveTexture")
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        CHECK_OGL_ERR("Texture glBindTexture")
    }
    else
    {
        LOG_ERROR("Failed activate texture: m_textureID is invalid");
    }
}

bool Texture::isValid() const
{
    return m_textureID != 0;
}

GLuint Texture::getTextureID() const
{
    return m_textureID;
}

void Texture::createTexture(const TextureDesc& desc)
{
    glGenTextures(1, &m_textureID);
    CHECK_OGL_ERR("Texture glGenTextures")
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    CHECK_OGL_ERR("Texture glBindTexture")
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    CHECK_OGL_ERR("Texture glTexParameteri GL_TEXTURE_WRAP_S")
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    CHECK_OGL_ERR("Texture glTexParameteri GL_TEXTURE_WRAP_T")
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_OGL_ERR("Texture glTexParameteri GL_TEXTURE_MAG_FILTER")
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    CHECK_OGL_ERR("Texture glTexParameteri GL_TEXTURE_MIN_FILTER")
    
    int width = 0;
    int height = 0;
    int numChannels = 0;
    unsigned char* data = stbi_load(
        desc.m_texturePath.c_str(), 
        &width, &height, 
        &numChannels, 0
        );
    
    if (data)
    {
        GLint format = GL_RGB;
        if (numChannels == 1)
        {
            format = GL_RED;
        }
        else if (numChannels == 4)
        {
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        CHECK_OGL_ERR("Texture glTexImage2D")
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_OGL_ERR("Texture glGenerateMipmap")
    }
    else
    {
        LOG_ERROR("Failed to load texture: {}", desc.m_texturePath);
        assert(false);
    }
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_OGL_ERR("Texture glBindTexture")
}

void Texture::destroyTexture()
{
    glDeleteTextures(1, &m_textureID);
    CHECK_OGL_ERR("Texture glDeleteTextures")
    m_textureID = 0;
}
