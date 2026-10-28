#pragma once

#include "entt.h"

#include "ComponentFactory/ComponentFactory.h"

class EntityHierarchyComponent
{
public:
    
    entt::entity m_parent   = entt::null;
    entt::entity m_child    = entt::null;
    entt::entity m_neighbor = entt::null;
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, EntityHierarchyComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, EntityHierarchyComponent);
    
};
