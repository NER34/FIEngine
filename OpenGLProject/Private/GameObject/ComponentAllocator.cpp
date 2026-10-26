#include "GameObject/ComponentAllocator.h"

#include <ranges>

#include "GameObject/ComponentFactory.h"

ComponentAllocator& ComponentAllocator::Get()
{
    static ComponentAllocator componentAllocator;
    return componentAllocator;
}

void ComponentAllocator::AddComponentDeserialized(
    const std::string& name,
    entt::registry& registry,
    entt::entity entity,
    const nlohmann::json& data,
    const std::unordered_map<uint32_t, entt::entity>& entityIDMap
    )
{
    if (auto it = m_componentFactories.find(name); it != m_componentFactories.end())
    {
        it->second->Create(registry, entity, data, entityIDMap);
    }
}

void ComponentAllocator::CopyComponents(
    const entt::registry& from, 
    entt::registry& to,
    const std::unordered_map<entt::entity, entt::entity>& entityIDMap
    )
{
    for (const auto& factory : m_componentFactories | std::views::values)
    {
        factory->Copy(from, to, entityIDMap);
    }
}

bool ComponentAllocator::IsValidComponentName(const std::string& name) const
{
    return m_componentFactories.contains(name);
}
