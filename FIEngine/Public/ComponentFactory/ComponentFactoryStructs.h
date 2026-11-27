#pragma once

#include "entt.h"

struct ComponentHandle
{
    entt::registry& m_registry;
    entt::entity m_entity;
};

struct ConstComponentHandle
{
    const entt::registry& m_registry;
    entt::entity m_entity;
};

class EntityIDMapping
{
public:

    void Map(entt::entity from, entt::entity to)
    {
        m_entityIDMap.insert({ from, to });
    }
    
    entt::entity Get(entt::entity from) const
    {
        if (auto it = m_entityIDMap.find(from); it != m_entityIDMap.end())
        {
            return it->second;
        }
        return entt::null;
    }
    
private:
    
    std::unordered_map<entt::entity, entt::entity> m_entityIDMap;

};