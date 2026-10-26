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
    
    std::vector<entt::entity> m_entities;
    
};
