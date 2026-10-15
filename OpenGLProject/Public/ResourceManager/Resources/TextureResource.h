#pragma once

#include <optional>

#include "Rendering/Texture.h"
#include "ResourceManager/ResourceBase.h"

class TextureResource : public ResourceBase
{
public:
    
    TextureResource() = default;
    ~TextureResource() override;
    
    void Reload() override;
    void Free() override;
    
    const Texture* GetTexture() const;
    Texture* GetTexture();
    
private:
    
    std::optional<Texture> m_texture;
    
};
