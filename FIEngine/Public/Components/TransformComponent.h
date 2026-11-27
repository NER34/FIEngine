#pragma once

#include "entt.h"

#include "Utilities/Utilities.h"
#include "ComponentFactory/ComponentFactory.h"

class TransformComponent
{
public:

    void UpdateGlobalTransform(const entt::registry& registry);
    
    void SetTransform(const Transform& transform);
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    
    const glm::mat4& GetGlobalTransform() const;
    const Transform& GetTransform() const;
    
    void AttachTo(TransformComponent& parent);

    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, TransformComponent);
    
private:
    
    friend class SceneManager;
    
    Transform m_localTransform;
    glm::mat4 m_globalTransform = glm::mat4(1.0f);
    
    bool m_bTransformModified = true;
    bool m_bTransformUpdated = false;

    entt::entity m_current  = entt::null;
    entt::entity m_parent   = entt::null;
    entt::entity m_child    = entt::null;
    entt::entity m_neighbor_r = entt::null;
    entt::entity m_neighbor_l = entt::null;
    
};
