#include "Components/TransformComponent.h"

#include "Scene/SceneManager.h"
#include "Utilities/ParsingHelpers.h"

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

void TransformComponent::SetTransform(const Transform& transform)
{
    m_bTransformModified = true;
    m_localTransform = transform;
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

void TransformComponent::AttachTo(TransformComponent& parent)
{
    auto& sceneManager = SceneManager::Get();

    for (auto* ancestor = &parent; ancestor; ancestor = sceneManager.GetComponent<TransformComponent>(ancestor->m_parent))
    {
        if (ancestor == this)
        {
            LOG_WARN("Trying to attach a transform to itself or one of its descendants");
            return;
        }
    }

    m_bTransformModified = true;

    {
        auto* parentTransform = sceneManager.GetComponent<TransformComponent>(m_parent);
        auto* neighborRTransform = sceneManager.GetComponent<TransformComponent>(m_neighbor_r);
        auto* neighborLTransform = sceneManager.GetComponent<TransformComponent>(m_neighbor_l);
    
        if (parentTransform && parentTransform->m_child == m_current)
        {
            parentTransform->m_child = m_neighbor_r;
        }
        if (neighborLTransform)
        {
            neighborLTransform->m_neighbor_r = m_neighbor_r;
        }
        if (neighborRTransform)
        {
            neighborRTransform->m_neighbor_l = m_neighbor_l;
        }
    }
    m_parent = parent.m_current;
    m_neighbor_r = parent.m_child;
    if (auto* neighborRTransform = sceneManager.GetComponent<TransformComponent>(m_neighbor_r))
    {
        neighborRTransform->m_neighbor_l = m_current;
    }
    m_neighbor_l = entt::null;
    parent.m_child = m_current;
    
    sceneManager.MarkSceneGraphModified();
}

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent)
{
    auto& component = handle.m_registry.get<TransformComponent>(handle.m_entity);
    
    ::DeserializeVector3(data, "position",  component.m_localTransform.m_position);
    ::DeserializeVector3(data, "rotation",  component.m_localTransform.m_rotation);
    ::DeserializeVector3(data, "scale",     component.m_localTransform.m_scale);

    component.m_current = handle.m_entity;
    ::DeserializeEntity(data, "parent",     mapping, component.m_parent);
    ::DeserializeEntity(data, "child",      mapping, component.m_child);
    ::DeserializeEntity(data, "neighbor_r",   mapping, component.m_neighbor_r);
    ::DeserializeEntity(data, "neighbor_l",   mapping, component.m_neighbor_l);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, TransformComponent)
{
    auto& to = handle_to.m_registry.get<TransformComponent>(handle_to.m_entity);
    const auto& from = handle_from.m_registry.get<TransformComponent>(handle_from.m_entity);
    
    to.m_localTransform = from.m_localTransform;
    to.m_current = handle_to.m_entity;

    if (from.m_parent != entt::null)
    {
        to.m_parent = mapping.Get(from.m_parent);
    }

    if (from.m_child != entt::null)
    {
        to.m_child = mapping.Get(from.m_child);
    }

    if (from.m_neighbor_r != entt::null)
    {
        to.m_neighbor_r = mapping.Get(from.m_neighbor_r);
    }

    if (from.m_neighbor_l != entt::null)
    {
        to.m_neighbor_l = mapping.Get(from.m_neighbor_l);
    }

    /*
    if (from.m_parent == entt::null)
    {
        auto& parent = handle_to.m_registry.get<TransformComponent>(to.m_parent);
        to.m_neighbor = parent.m_child;
        parent.m_child = handle_to.m_entity;
    }
    */
    
}

REGISTER_COMPONENT_FACTORY(TransformComponent, TransformComponent::Deserialize, TransformComponent::Copy)
