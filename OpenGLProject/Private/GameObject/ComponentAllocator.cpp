#include "GameObject/ComponentAllocator.h"

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

bool ComponentAllocator::IsValidComponentName(const std::string& name)
{
    return m_componentFactories.contains(name);
}
