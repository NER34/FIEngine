#pragma once

#include "entt.h"

#include "Utilities/Utilities.h"
#include "ComponentFactory/ComponentFactory.h"

class TransformComponent
{
public:

    Transform m_transform;
    bool m_bModified = false;

    entt::entity m_parent   = entt::null;
    entt::entity m_child    = entt::null;
    entt::entity m_neighbor = entt::null;

    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, TransformComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, TransformComponent);
    
};
