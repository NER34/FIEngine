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
        CHECK_OGL_ERR(glActiveTexture(textureUnit))
        CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, m_textureID))
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
    CHECK_OGL_ERR(glGenTextures(1, &m_textureID))
    CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, m_textureID))
    
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
    
    if (desc.data)
    {
        GLint format = GL_RGB;
        if (desc.numChannels == 1)
        {
            format = GL_RED;
        }
        else if (desc.numChannels == 4)
        {
            format = GL_RGBA;
        }
        
        CHECK_OGL_ERR(glTexImage2D(
            GL_TEXTURE_2D, 0, format, 
            desc.dimensions[0], desc.dimensions[1], 
            0, format, GL_UNSIGNED_BYTE, desc.data
            ))
        CHECK_OGL_ERR(glGenerateMipmap(GL_TEXTURE_2D))
    }
    else
    {
        LOG_ERROR("Failed to create texture: Invalid data");
        assert(false);
    }

    CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, 0))
}

void Texture::destroyTexture()
{
    CHECK_OGL_ERR(glDeleteTextures(1, &m_textureID))
    m_textureID = 0;
}
