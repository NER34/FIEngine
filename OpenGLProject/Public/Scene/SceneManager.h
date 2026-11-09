#pragma once

#include <entt.h>

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/SceneResource.h"

class SceneManager
{
public:
    
    static SceneManager& Get();
    
    void Initialize();
    void Update();
    void Deinitialize();

    entt::entity GetRootEntity() const;
    void MarkSceneGraphModified();
    entt::entity SpawnScene(const ResourceHandle<SceneResource>& scene, entt::entity parent = entt::null);
    
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
    
    template <typename TComp>
    auto* GetComponent(entt::entity entity)
    {
        return m_registry.valid(entity)
            ? &m_registry.get<TComp>(entity)
            : nullptr;
    }
    
private:
    
    entt::registry m_registry;
    entt::entity m_sceneRoot = entt::null;
    bool m_sceneGraphModified = false;
    ResourceHandle<SceneResource> m_sceneHandle;
};
