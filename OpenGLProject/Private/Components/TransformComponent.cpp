#include "Components/TransformComponent.h"

#include "Utilities/Logger.h"

namespace
{
    void DeserializeVector3(const nlohmann::json& data, const char* key, glm::vec3& outVector)
    {
        if (!data.contains(key))
        {
            return;
        }

        const nlohmann::json& value = data[key];
        if (!value.is_array() || value.size() != 3)
        {
            LOG_ERROR("Transform component {} must contain three values", key);
            return;
        }

        for (std::size_t index = 0; index < 3; ++index)
        {
            if (!value[index].is_number())
            {
                LOG_ERROR("Transform component {} has an invalid value", key);
                return;
            }
            outVector[index] = value[index];
        }
    }

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
            LOG_ERROR("Transform component {} must be an unsigned integer or null", key);
            outEntity = entt::null;
            return;
        }

        uint32_t rawEntityID = data[key];
        outEntity = entityIDMap.Get(static_cast<entt::entity>(rawEntityID));
        if (outEntity == entt::null)
        {
            LOG_ERROR("Transform component {} references unknown entity id {}", key, rawEntityID);
        }
    }
}

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent)
{
    ::DeserializeVector3(data, "position", component.m_transform.m_position);
    ::DeserializeVector3(data, "rotation", component.m_transform.m_rotation);
    ::DeserializeVector3(data, "scale", component.m_transform.m_scale);

    ::DeserializeEntity(data, "parent",     mapping, component.m_parent);
    ::DeserializeEntity(data, "child",      mapping, component.m_child);
    ::DeserializeEntity(data, "neighbor",   mapping, component.m_neighbor);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, TransformComponent)
{
    to.m_transform = from.m_transform;
    to.m_bModified = from.m_bModified;

    to.m_parent     = mapping.Get(from.m_parent);
    to.m_child      = mapping.Get(from.m_child);
    to.m_neighbor   = mapping.Get(from.m_neighbor);
}

REGISTER_COMPONENT_FACTORY(TransformComponent, TransformComponent::Deserialize, TransformComponent::Copy)
