#include "ResourceManager/Resources/SceneResource.h"

#include <fstream>
#include <json.h>

#include "GameObject/ComponentAllocator.h"
#include "Scene/SceneManager.h"
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
    
    auto& registry = SceneManager::Get().GetRegistry();
    std::unordered_map<uint32_t, entt::entity> entityIDMap;
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
        if (entityIDMap.contains(rawEntityID))
        {
            LOG_ERROR("Invalid scene file {}: duplicated entity id", fullPath);
            assert(false);
            Free();
            return;
        }
        entt::entity entityID = registry.create();
        m_entities.push_back(entityID);
        entityIDMap.insert({ entity, entityID });
    }
    
    auto& componentAllocator = ComponentAllocator::Get();
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
        if (!componentAllocator.IsValidComponentName(type))
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
        if (!entityIDMap.contains(rawEntityID))
        {
            LOG_ERROR("Invalid scene file {}: unknown entity id {}", fullPath, rawEntityID);
            assert(false);
            Free();
            return;
        }

        componentAllocator.AddComponentDeserialized(
            type, registry, entityIDMap[rawEntityID], component["data"], entityIDMap
            );
    }
}

void SceneResource::Free()
{
    entt::registry& registry = SceneManager::Get().GetRegistry();
    for (auto entity : m_entities)
    {
        registry.destroy(entity);
    }
    m_entities.clear();
}
