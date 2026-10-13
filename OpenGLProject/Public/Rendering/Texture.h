#pragma once

#include <string>

#include "glad/glad.h"

struct TextureDesc
{
    //std::string m_texturePath;
    int dimensions[3] = { 0, 0, 0 };
    int numChannels = 0;
    unsigned char* data;
};

class Texture
{
public:
    
    Texture() = default;
    Texture(const TextureDesc& desc);
    
    Texture(const Texture& other) = default;
    Texture& operator=(const Texture& other) = default;
    
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;
    
    ~Texture();
    
    void setup(const TextureDesc& desc);
    void activate(GLenum textureUnit = GL_TEXTURE0) const;
    
    bool isValid() const;
    
    GLuint getTextureID() const;
    
protected:
    
    void createTexture(const TextureDesc& desc);
    void destroyTexture();
    
private:
    
    GLuint m_textureID = 0;
    
};
