#pragma once
#include <vector>

#include "entt.h"

#include "ResourceManager/ResourceBase.h"

class SceneResource : public ResourceBase
{
public:
    
    SceneResource() = default;
    ~SceneResource() override;
    
    void Reload() override;
    void Free() override;

private:
    
    friend class SceneManager;
    void CopyToRegistry(entt::registry& other) const;
    
    entt::registry m_registry;
    
};
