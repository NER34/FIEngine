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
    
    virtual void Copy(
        const entt::registry& from,
        entt::registry& to,
        const std::unordered_map<entt::entity, entt::entity>& entityIDMap
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
    
    using CopyFunc = std::function<void(
        const TComponent&,
        TComponent&,
        const std::unordered_map<entt::entity, entt::entity>&
        )>;

public:
    
    ComponentFactory(DeserializeFunc deserialize, CopyFunc copy) 
        : m_deserialize(deserialize) 
        , m_copy(copy)
    {}
    
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
    
    void Copy(
        const entt::registry& from,
        entt::registry& to,
        const std::unordered_map<entt::entity, entt::entity>& entityIDMap
    ) const final
    {
        from.view<TComponent>().each([&](entt::entity entity, const TComponent& component)
        {
            TComponent& componentCopy = to.emplace<TComponent>(entityIDMap.at(entity), component);
            m_copy(component, componentCopy, entityIDMap);
        });
    }

private:
    
    DeserializeFunc m_deserialize;  
    CopyFunc        m_copy;  
    static bool s_registered;
    
};
