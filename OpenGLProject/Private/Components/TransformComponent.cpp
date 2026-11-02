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

void TransformComponent::UpdateGlobalTransform(const entt::registry& registry)
{
    m_bTransformUpdated = m_bTransformModified;
    if (m_parent == entt::null)
    {
        if (m_bTransformUpdated)
        {    
            m_globalTransform = m_localTransform.GetModelMatrix();
        }
    }
    else
    {
        const TransformComponent& parent = registry.get<TransformComponent>(m_parent);
        m_bTransformUpdated = m_bTransformUpdated || parent.m_bTransformUpdated;
        if (m_bTransformUpdated)
        {    
            m_globalTransform = parent.m_globalTransform * m_localTransform.GetModelMatrix();
        }
    }
    m_bTransformModified = false;
}

void TransformComponent::SetPosition(const glm::vec3& position)
{
    m_bTransformModified = true;
    m_localTransform.m_position = position;
}

void TransformComponent::SetRotation(const glm::vec3& rotation)
{
    m_bTransformModified = true;
    m_localTransform.m_rotation = rotation;
}

void TransformComponent::SetScale(const glm::vec3& scale)
{
    m_bTransformModified = true;
    m_localTransform.m_scale = scale;
}

const glm::mat4& TransformComponent::GetGlobalTransform() const
{
    return m_globalTransform;    
}

const Transform& TransformComponent::GetTransform() const
{
    return m_localTransform;
}

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent)
{
    auto& component = handle.m_registry.get<TransformComponent>(handle.m_entity);
    
    ::DeserializeVector3(data, "position", component.m_localTransform.m_position);
    ::DeserializeVector3(data, "rotation", component.m_localTransform.m_rotation);
    ::DeserializeVector3(data, "scale", component.m_localTransform.m_scale);

    ::DeserializeEntity(data, "parent",     mapping, component.m_parent);
    ::DeserializeEntity(data, "child",      mapping, component.m_child);
    ::DeserializeEntity(data, "neighbor",   mapping, component.m_neighbor);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, TransformComponent)
{
    auto& to = handle_to.m_registry.get<TransformComponent>(handle_to.m_entity);
    const auto& from = handle_from.m_registry.get<TransformComponent>(handle_from.m_entity);
    
    to.m_localTransform = from.m_localTransform;

    to.m_parent = mapping.Get(from.m_parent);

    if (from.m_child != entt::null)
    {
        to.m_child = mapping.Get(from.m_child);
    }

    if (from.m_neighbor != entt::null)
    {
        to.m_neighbor = mapping.Get(from.m_neighbor);
    }

    if (from.m_parent == entt::null)
    {
        auto& parent = handle_to.m_registry.get<TransformComponent>(to.m_parent);
        to.m_neighbor = parent.m_child;
        parent.m_child = handle_to.m_entity;
    }
}

REGISTER_COMPONENT_FACTORY(TransformComponent, TransformComponent::Deserialize, TransformComponent::Copy)
