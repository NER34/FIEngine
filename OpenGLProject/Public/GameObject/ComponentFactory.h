#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "entt.h"
#include "json.h"

class IComponentFactory
{
public:
    virtual ~IComponentFactory() = default;
    virtual void Create(
        entt::registry& registry,
        entt::entity entity,
        const nlohmann::json& data,
        const std::unordered_map<uint32_t, entt::entity>& entityIDMap
        ) const = 0;
};

template <typename TComponent>
class ComponentFactory : public IComponentFactory
{    
    using DeserializeFunc = std::function<void(
        TComponent&, 
        const nlohmann::json&, 
        const std::unordered_map<uint32_t, entt::entity>&
        )>;

public:
    
    ComponentFactory(DeserializeFunc deserialize) : m_deserialize(deserialize) {}
    
    void Create(
        entt::registry& registry,
        entt::entity entity,
        const nlohmann::json& data,
        const std::unordered_map<uint32_t, entt::entity>& entityIDMap
        ) const final
    {
        TComponent& component = registry.emplace_or_replace<TComponent>(entity);
        m_deserialize(component, data, entityIDMap);
    }

private:
    
    DeserializeFunc m_deserialize;  
    static bool s_registered;
    
};
