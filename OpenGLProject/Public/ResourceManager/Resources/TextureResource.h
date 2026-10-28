#pragma once

#include "glad/glad.h"

#include "ResourceManager/ResourceBase.h"

struct TextureDesc
{
    int dimensions[3] = { 0, 0, 0 };
    int numChannels = 0;
    unsigned char* data = nullptr;
};

class TextureResource : public ResourceBase
{
public:
    
    TextureResource() = default;
    ~TextureResource() override;
    
    TextureResource(const TextureResource& other) = delete;
    TextureResource& operator=(const TextureResource& other) = delete;
    
    void Reload() override;
    void Free() override;
    
    void Activate(GLenum textureUnit = GL_TEXTURE0) const;
    
    bool IsValid() const;
    GLuint GetTextureID() const;
    
private:
    
    void CreateTexture(const TextureDesc& desc);
    
    GLuint m_textureID = 0;
    
};
