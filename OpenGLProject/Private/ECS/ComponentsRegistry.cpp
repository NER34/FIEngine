#include "ComponentsRegistry.h"

#include <ranges>
#include "entt.h"

void ComponentsRegistry::RemoveEntityFromAllPools(EntityID entity)
{
    for (auto& val : m_pools | std::views::values)
    {
        val->Remove(entity);
    }
}

void ComponentsRegistry::Clear()
{
    m_pools.clear();
}
