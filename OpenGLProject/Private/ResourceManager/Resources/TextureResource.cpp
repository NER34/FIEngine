#include "ResourceManager/Resources/TextureResource.h"

#include "stb_image.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

TextureResource::~TextureResource()
{
    TextureResource::Free();
}

void TextureResource::Reload()
{
    Free();
    
    std::string texturePath = (getResourcesPath() / GetFilePath()).string();
    TextureDesc textureDesc;
    textureDesc.data = stbi_load(
        texturePath.c_str(), 
        &textureDesc.dimensions[0], &textureDesc.dimensions[1], 
        &textureDesc.numChannels, 0
        );
    
    if (textureDesc.data)
    {
        m_texture.emplace(textureDesc);
        stbi_image_free(textureDesc.data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: {}", texturePath);
        assert(false);
    }
}

void TextureResource::Free()
{
    m_texture.reset();
}

const Texture* TextureResource::GetTexture() const
{
    return m_texture ? &m_texture.value() : nullptr;
}

Texture* TextureResource::GetTexture()
{
    return m_texture ? &m_texture.value() : nullptr;
}
