#include "Components/EntityHierarchyComponent.h"

#include "Utilities/Logger.h"

namespace
{
    void DeserializeEntity(
        const nlohmann::json& data,
        const char* key,
        const EntityIDMapping& entityIDMap,
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
        outEntity = entityIDMap.Get(static_cast<entt::entity>(rawEntityID));
        if (outEntity == entt::null)
        {
            LOG_ERROR("Entity hierarchy component {} references unknown entity id {}", key, rawEntityID);
        }
    }
}

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, EntityHierarchyComponent)
{
    ::DeserializeEntity(data, "parent",     mapping, component.m_parent);
    ::DeserializeEntity(data, "child",      mapping, component.m_child);
    ::DeserializeEntity(data, "neighbor",   mapping, component.m_neighbor);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, EntityHierarchyComponent)
{
    to.m_parent     = mapping.Get(from.m_parent);
    to.m_child      = mapping.Get(from.m_child);
    to.m_neighbor   = mapping.Get(from.m_neighbor);
}

REGISTER_COMPONENT_FACTORY(EntityHierarchyComponent, EntityHierarchyComponent::Deserialize, EntityHierarchyComponent::Copy)
