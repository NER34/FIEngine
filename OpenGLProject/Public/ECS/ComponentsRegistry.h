#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentPool.h"

class ComponentsRegistry
{
public:
    
    template<typename TComponent, typename... TArguments>
    TComponent& Emplace(EntityID entity, TArguments&&... arguments)
    {
        auto poolID = std::type_index(typeid(TComponent));
        if (!m_pools.contains(poolID))
        {
            m_pools.emplace(poolID, std::make_unique<ComponentPool<TComponent>>());
        }
        auto* pool = static_cast<ComponentPool<TComponent>*>(m_pools[poolID].get());
        return pool->Emplace(entity, std::forward<TArguments>(arguments)...);
    }

    template<typename TComponent>
    TComponent* Get(EntityID entity)
    {
        auto poolID = std::type_index(typeid(TComponent));
        if (auto it = m_pools.find(poolID); it != m_pools.end())
        {
            auto* pool = static_cast<ComponentPool<TComponent>*>(it->second.get());
            return pool->Get(entity);
        }
        return nullptr;
    }

    template<typename TComponent>
    const TComponent* Get(EntityID entity) const
    {
        auto poolID = std::type_index(typeid(TComponent));
        if (auto it = m_pools.find(poolID); it != m_pools.end())
        {
            const auto* pool = static_cast<const ComponentPool<TComponent>*>(it->second.get());
            return pool->Get(entity);
        }
        return nullptr;
    }

    template<typename TComponent>
    bool Has(EntityID entity) const
    {
        auto poolID = std::type_index(typeid(TComponent));
        if (auto it = m_pools.find(poolID); it != m_pools.end())
        {
            const auto* pool = static_cast<const ComponentPool<TComponent>*>(it->second.get());
            return pool->Has(entity);
        }
        return false;
    }

    template<typename TComponent>
    bool Remove(EntityID entity)
    {
        auto poolID = std::type_index(typeid(TComponent));
        if (auto it = m_pools.find(poolID); it != m_pools.end())
        {
            const auto* pool = static_cast<const ComponentPool<TComponent>*>(it->second.get());
            return pool->Remove(entity);
        }
        return false;
    }

    /*
    template<typename TMaskComponent, typename... TComponents>
    auto View();
    */

    void RemoveEntityFromAllPools(EntityID entity);
    void Clear();

private:
    
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_pools;
    
};
