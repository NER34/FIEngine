#include "GameObject/Components/EntityHierarchyComponent.h"

#include "GameObject/ComponentAllocator.h"
#include "GameObject/ComponentFactory.h"
#include "Utilities/Logger.h"

namespace
{
    void DeserializeEntity(
        const nlohmann::json& data,
        const char* key,
        const std::unordered_map<uint32_t, entt::entity>& entityIDMap,
        entt::entity& outEntity
        )
    {
        if (!data.contains(key) || data[key].is_null())
        {
            outEntity = entt::null;
            return;
        }

        if (!data[key].is_number_unsigned())
        {
            LOG_ERROR("Entity hierarchy component {} must be an unsigned integer or null", key);
            outEntity = entt::null;
            return;
        }

        uint32_t rawEntityID = data[key];
        if (auto it = entityIDMap.find(rawEntityID); it != entityIDMap.end())
        {
            outEntity = it->second;
        }
        else
        {
            LOG_ERROR("Entity hierarchy component {} references unknown entity id {}", key, rawEntityID);
            outEntity = entt::null;
        }
    }
}

void Deserialize(
    EntityHierarchyComponent& component,
    const nlohmann::json& data,
    const std::unordered_map<uint32_t, entt::entity>& entityIDMap
    )
{
    DeserializeEntity(data, "parent", entityIDMap, component.m_parent);
    DeserializeEntity(data, "child", entityIDMap, component.m_child);
    DeserializeEntity(data, "neighbor", entityIDMap, component.m_neighbor);
}

REGISTER_COMPONENT_FACTORY(EntityHierarchyComponent, Deserialize)
