#include "ResourceManager/Resources/SceneResource.h"

#include <fstream>
#include <json.h>

#include "ComponentFactory/ComponentFactory.h"
#include "Utilities/Logger.h"

SceneResource::~SceneResource()
{
    SceneResource::Free();
}

void SceneResource::Reload()
{
    Free();

    const std::string fullPath = GetFullFilePath();
    std::ifstream sceneFile(fullPath);
    if (!sceneFile.is_open())
    {
        LOG_ERROR("Failed to open scene file: {}", fullPath);
        assert(false);
        return;
    }

    nlohmann::json sceneAsset;
    try
    {
        sceneFile >> sceneAsset;
    }
    catch (const nlohmann::json::exception& exception)
    {
        LOG_ERROR("Failed to parse scene file {}: {}", fullPath, exception.what());
        assert(false);
        return;
    }
    
    if (!sceneAsset.contains("entities") || !sceneAsset["entities"].is_array())
    {
        LOG_ERROR("Invalid scene file {}: array of entities is missing", fullPath);
        assert(false);
        return;
    }
    if (!sceneAsset.contains("components") || !sceneAsset["components"].is_array())
    {
        LOG_ERROR("Invalid scene file {}: array of components is missing", fullPath);
        assert(false);
        return;
    }
    
    EntityIDMapping mapping;
    for (const auto& entity : sceneAsset["entities"])
    {
        if (!entity.is_number_unsigned())
        {
            LOG_ERROR("Invalid scene file {}: entity id is not an unsigned number", fullPath);
            assert(false);
            Free();
            return;
        }
        uint32_t rawEntityID = entity;
        if (mapping.Get(static_cast<entt::entity>(rawEntityID)) != entt::null)
        {
            LOG_ERROR("Invalid scene file {}: duplicated entity id", fullPath);
            assert(false);
            Free();
            return;
        }

        mapping.Map(static_cast<entt::entity>(rawEntityID), m_registry.create());
    }

    auto& componentRegistry = ComponentRegistry::Get();
    for (const auto& component : sceneAsset["components"])
    {
        if (!component.is_object())
        {
            LOG_ERROR("Invalid scene file {}: invalid component", fullPath);
            assert(false);
            Free();
            return;
        }
        if (!component.contains("type") || !component["type"].is_string())
        {
            LOG_ERROR("Invalid scene file {}: component type missing", fullPath);
            assert(false);
            Free();
            return;
        }
        std::string type = component["type"];
        IComponentFactory* componentFactory = componentRegistry.GetObject(type);
        if (!componentFactory)
        {
            LOG_ERROR("Invalid scene file {}: unknown component type {}", fullPath, type);
            assert(false);
            Free();
            return;
        }
        if (!component.contains("entity") || !component["entity"].is_number_unsigned())
        {
            LOG_ERROR("Invalid scene file {}: component entity missing", fullPath);
            assert(false);
            Free();
            return;
        }
        if (!component.contains("data") || !component["data"].is_object())
        {
            LOG_ERROR("Invalid scene file {}: component data missing", fullPath);
            assert(false);
            Free();
            return;
        }
        uint32_t rawEntityID = component["entity"];
        entt::entity entity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (entity == entt::null)
        {
            LOG_ERROR("Invalid scene file {}: unknown entity id {}", fullPath, rawEntityID);
            assert(false);
            Free();
            return;
        }

        componentFactory->CreateFromAsset({ m_registry, entity }, component["data"], mapping);
    }
}

void SceneResource::Free()
{
    m_registry.clear();
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
