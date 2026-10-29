#include "ResourceManager/Resources/SceneResource.h"

#include <cassert>

#include <json.h>

#include "ComponentFactory/ComponentFactory.h"
#include "Utilities/Logger.h"

SceneResource::~SceneResource()
{
    SceneResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, SceneResource)
{
    resource.Free();

    if (!data.contains("entities") || !data["entities"].is_array())
    {
        LOG_ERROR("Invalid scene file: array of entities is missing");
        assert(false);
        return false;
    }
    if (!data.contains("components") || !data["components"].is_array())
    {
        LOG_ERROR("Invalid scene file: array of components is missing");
        assert(false);
        return false;
    }
    
    EntityIDMapping mapping;
    for (const auto& entity : data["entities"])
    {
        if (!entity.is_number_unsigned())
        {
            LOG_ERROR("Invalid scene file: entity id is not an unsigned number");
            assert(false);
            resource.Free();
            return false;
        }
        uint32_t rawEntityID = entity;
        if (mapping.Get(static_cast<entt::entity>(rawEntityID)) != entt::null)
        {
            LOG_ERROR("Invalid scene file: duplicated entity id");
            assert(false);
            resource.Free();
            return false;
        }

        mapping.Map(static_cast<entt::entity>(rawEntityID), resource.m_registry.create());
    }

    auto& componentRegistry = ComponentRegistry::Get();
    for (const auto& component : data["components"])
    {
        if (!component.is_object())
        {
            LOG_ERROR("Invalid scene file: invalid component");
            assert(false);
            resource.Free();
            return false;
        }
        if (!component.contains("type") || !component["type"].is_string())
        {
            LOG_ERROR("Invalid scene file: component type missing");
            assert(false);
            resource.Free();
            return false;
        }
        std::string type = component["type"];
        IComponentFactory* componentFactory = componentRegistry.GetObject(type);
        if (!componentFactory)
        {
            LOG_ERROR("Invalid scene file: unknown component type {}", type);
            assert(false);
            resource.Free();
            return false;
        }
        if (!component.contains("entity") || !component["entity"].is_number_unsigned())
        {
            LOG_ERROR("Invalid scene file: component entity missing");
            assert(false);
            resource.Free();
            return false;
        }
        if (!component.contains("data") || !component["data"].is_object())
        {
            LOG_ERROR("Invalid scene file: component data missing");
            assert(false);
            resource.Free();
            return false;
        }
        uint32_t rawEntityID = component["entity"];
        entt::entity entity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (entity == entt::null)
        {
            LOG_ERROR("Invalid scene file: unknown entity id {}", rawEntityID);
            assert(false);
            resource.Free();
            return false;
        }

        componentFactory->CreateFromAsset({ resource.m_registry, entity }, component["data"], mapping);
    }
    return true;
}

void SceneResource::Free()
{
    m_registry.clear();
}

entt::registry& SceneResource::GetRegistry()
{
    return m_registry;
}

const entt::registry& SceneResource::GetRegistry() const
{
    return m_registry;
}

void SceneResource::CopyToRegistry(entt::registry& other) const
{
    EntityIDMapping mapping;
    for (auto [entity] : m_registry.storage<entt::entity>()->each())
    {
        mapping.Map(entity, other.create());
    }
    ComponentRegistry::Get().ForEachObject([&](const std::string&, const IComponentFactory& factory)
    {
        factory.CloneAll(m_registry, other, mapping);
    });
}

REGISTER_ASSET_FACTORY(SceneResource, SceneResource::Deserialize)
