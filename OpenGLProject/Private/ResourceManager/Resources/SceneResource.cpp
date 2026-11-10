#include "ResourceManager/Resources/SceneResource.h"

#include <cassert>

#include <json.h>

#include "AssetManager/AssetManager.h"
#include "ComponentFactory/ComponentFactory.h"
#include "Utilities/Logger.h"
#include "Utilities/ParsingHelpers.h"

SceneResource::~SceneResource()
{
    SceneResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, SceneResource)
{
    resource.Free();

    if (!HasArrayField(data, "entities"))
    {
        LOG_ERROR("Invalid scene file: array of entities is missing");
        assert(false);
        return false;
    }
    if (!HasArrayField(data, "components"))
    {
        LOG_ERROR("Invalid scene file: array of components is missing");
        assert(false);
        return false;
    }
    if (!IsOptionalArrayField(data, "scenes"))
    {
        LOG_ERROR("Invalid scene file: scenes is not an array");
        assert(false);
        return false;
    }
    
    EntityIDMapping mapping;
    
    if (!ParseEntities(resource, data, mapping))
    {
        LOG_ERROR("Invalid scene file: failed to parse entities");
        assert(false);
        resource.Free();
        return false;
    }
    
    if (!ParseComponents(resource, data, mapping))
    {
        LOG_ERROR("Invalid scene file: failed to parse components");
        assert(false);
        resource.Free();
        return false;
    }
    
    if (!ParseSubScenes(resource, data, mapping))
    {
        LOG_ERROR("Invalid scene file: failed to parse sub scenes");
        assert(false);
        resource.Free();
        return false;
    }
    
    return true;
}

void SceneResource::Free()
{
    m_registry.clear();
    m_subScenes.clear();
}

bool SceneResource::ParseEntities(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping)
{
    if (!HasUnsignedField(data, "root"))
    {
        LOG_ERROR("Invalid scene file: root entity missing");
        assert(false);
        return false;
    }
    for (const auto& entity : data["entities"])
    {
        if (!entity.is_number_unsigned())
        {
            LOG_ERROR("Invalid scene file: entity id is not an unsigned number");
            assert(false);
            return false;
        }
        uint32_t rawEntityID = entity;
        if (mapping.Get(static_cast<entt::entity>(rawEntityID)) != entt::null)
        {
            LOG_ERROR("Invalid scene file: duplicated entity id");
            assert(false);
            return false;
        }

        mapping.Map(static_cast<entt::entity>(rawEntityID), resource.m_registry.create());
    }
    resource.m_rootEntity = mapping.Get(data["root"]);
    if (resource.m_rootEntity == entt::null)
    {
        LOG_ERROR("Invalid scene file: invalid root entity");
        assert(false);
        return false;
    }
    return true;
}

bool SceneResource::ParseComponents(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping)
{
    auto& componentRegistry = ComponentRegistry::Get();
    for (const auto& component : data["components"])
    {
        if (!component.is_object())
        {
            LOG_ERROR("Invalid scene file: invalid component");
            assert(false);
            return false;
        }
        if (!HasStringField(component, "type"))
        {
            LOG_ERROR("Invalid scene file: component type missing");
            assert(false);
            return false;
        }
        std::string type = component["type"];
        IComponentFactory* componentFactory = componentRegistry.GetObject(type);
        if (!componentFactory)
        {
            LOG_ERROR("Invalid scene file: unknown component type {}", type);
            assert(false);
            return false;
        }
        if (!HasUnsignedField(component, "entity"))
        {
            LOG_ERROR("Invalid scene file: component entity missing");
            assert(false);
            return false;
        }
        if (!HasObjectField(component, "data"))
        {
            LOG_ERROR("Invalid scene file: component data missing");
            assert(false);
            return false;
        }
        uint32_t rawEntityID = component["entity"];
        entt::entity entity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (entity == entt::null)
        {
            LOG_ERROR("Invalid scene file: unknown entity id {}", rawEntityID);
            assert(false);
            return false;
        }

        componentFactory->CreateFromAsset({ resource.m_registry, entity }, component["data"], mapping);
    }
    return true;
}

bool SceneResource::ParseSubScenes(SceneResource& resource, const nlohmann::json& data, EntityIDMapping& mapping)
{
    if (!data.contains("scenes"))
    {
        return true;
    }
    
    auto& assetManager = AssetManager::Get();
    for (const auto& scene : data["scenes"])
    {
        if (!scene.is_object())
        {
            LOG_ERROR("Invalid scene file: invalid sub scene");
            assert(false);
            return false;
        }
        SubSceneDesc desc;
        if (!HasUnsignedField(scene, "parent"))
        {
            LOG_ERROR("Invalid scene file: sub scene parent missing");
            assert(false);
            return false;
        }
        if (!HasObjectField(scene, "transform"))
        {
            LOG_ERROR("Invalid scene file: sub scene transform missing");
            assert(false);
            return false;
        }
        if (!HasStringField(scene, "path"))
        {
            LOG_ERROR("Invalid scene file: sub scene path missing");
            assert(false);
            return false;
        }
        
        const auto& transformData = scene["transform"];
        DeserializeEntity(scene, "parent", mapping, desc.m_parent);
        if (desc.m_parent == entt::null)
        {
            LOG_ERROR("Invalid scene file: invalid sub scene parent");
            assert(false);
            return false;
        }
        
        DeserializeVector3(transformData, "position", desc.m_transform.m_position);
        DeserializeVector3(transformData, "rotation", desc.m_transform.m_rotation);
        DeserializeVector3(transformData, "scale", desc.m_transform.m_scale);
        
        // TODO: This code can lead to circular dependency of scene assets
        // I won't fix it for now, but it's good to keep it in mind 
        if (!assetManager.Load(scene["path"], desc.m_scene))
        {
            LOG_ERROR("Invalid scene file: failed to load sub scene");
            assert(false);
            return false;
        }
        
        resource.m_subScenes.push_back(desc);
    }
    return true;
}

REGISTER_ASSET_FACTORY(SceneResource, SceneResource::Deserialize)
