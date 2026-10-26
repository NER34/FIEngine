#pragma once

#include "entt.h"

class EntityHierarchyComponent
{
public:
    entt::entity m_parent   = entt::null;
    entt::entity m_child    = entt::null;
    entt::entity m_neighbor = entt::null;
};
