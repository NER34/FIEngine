#pragma once
#include <vector>

#include "entt.h"

#include "AssetManager/AssetFactory.h"
#include "ComponentFactory/ComponentFactoryStructs.h"
#include "ResourceManager/ResourceHandle.h"
#include "Utilities/Utilities.h"


class SceneResource : public ResourceBase
{
public:
    
    SceneResource() = default;
    ~SceneResource() override;
    
    void Free() override;
    
    //entt::registry& GetRegistry();
    //const entt::registry& GetRegistry() const;
    
    static bool ParseEntities(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping);
    static bool ParseComponents(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping);
    static bool ParseSubScenes(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping);
    
    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, SceneResource);

private:
    
    friend class SceneManager;
    
    struct SubSceneDesc
    {
        entt::entity m_parent;
        Transform m_transform;
        ResourceHandle<SceneResource> m_scene;
    };
    
    std::vector<SubSceneDesc> m_subScenes;
    entt::registry m_registry;
    entt::entity m_rootEntity = entt::null;
};
