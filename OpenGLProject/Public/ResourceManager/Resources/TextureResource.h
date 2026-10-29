#pragma once

#include "glad/glad.h"

#include "AssetManager/AssetFactory.h"
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
    
    void Free() override;
    
    void Activate(GLenum textureUnit = GL_TEXTURE0) const;
    
    bool IsValid() const;
    GLuint GetTextureID() const;
    
    void CreateTexture(const TextureDesc& desc);
    
    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, TextureResource);
    
private:
    
    GLuint m_textureID = 0;
    
};
