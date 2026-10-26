#pragma once

#include <entt.h>

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/SceneResource.h"

class SceneManager
{
public:
    
    static SceneManager& Get();
    
    ~SceneManager();
    
    void InstantiateScene(const ResourceHandle<SceneResource>& scene);
    
    template <typename ... TComp>
    auto View() const
    {
        return m_registry.view<TComp...>();
    }
    
    template <typename ... TComp>
    auto View()
    {
        return m_registry.view<TComp...>();
    }
    
    void Free();
    
private:
    
    entt::registry m_registry;
    
};
