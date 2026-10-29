#pragma once
#include <vector>

#include "entt.h"

#include "AssetManager/AssetFactory.h"
#include "ResourceManager/ResourceBase.h"

class SceneResource : public ResourceBase
{
public:
    
    SceneResource() = default;
    ~SceneResource() override;
    
    void Free() override;
    
    entt::registry& GetRegistry();
    const entt::registry& GetRegistry() const;
    
    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, SceneResource);

private:
    
    friend class SceneManager;
    void CopyToRegistry(entt::registry& other) const;
    
    entt::registry m_registry;
    
};
