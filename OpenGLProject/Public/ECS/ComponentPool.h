#pragma once

#include <unordered_map>

typedef uint64_t EntityID;

class IComponentPool
{
public:
    virtual ~IComponentPool() = default;

    virtual bool Remove(EntityID entity) = 0;
    virtual bool Contains(EntityID entity) const = 0;
    virtual void Clear() = 0;
};

template<typename TComponent>
class ComponentPool : public IComponentPool
{
public:
    
    TComponent* Emplace(EntityID entity, auto&&... arguments)
    {
        if (m_entityToIndex.contains(entity))
        {
            return nullptr;
        }
        
        size_t index = m_components.size();
        m_components.push_back(std::forward<decltype(arguments)>(arguments)...);
        m_entities.push_back(entity);
        m_entityToIndex[entity] = index;
        
        return &m_components[index];
    }
    
    TComponent* Get(EntityID entity)
    {
        if (auto it = m_entityToIndex.find(entity); it != m_entityToIndex.end())
        {
            return &m_components[it->second];
        }
        return nullptr;
    }
    
    const TComponent* Get(EntityID entity) const
    {
        if (auto it = m_entityToIndex.find(entity); it != m_entityToIndex.end())
        {
            return &m_components[it->second];
        }
        return nullptr;
    }

    bool Contains(EntityID entity) const override
    {
        return m_entityToIndex.contains(entity);
    }
    
    bool Remove(EntityID entity) override
    {
        if (auto it = m_entityToIndex.find(entity); it != m_entityToIndex.end())
        {
            size_t index = it->second;
            size_t last_index = m_components.size() - 1;
            std::swap(m_components[index], m_components[last_index]);
            
            m_entityToIndex.erase(it);
            m_components.pop_back();
            
            return true;
        }
        return false;
    }
    
    void Clear() override
    {
        m_components.clear();
        m_entityToIndex.clear();
    }

    auto begin()
    {
        
    }
    
    auto end()
    {
        
    }

private:
    
    std::vector<TComponent> m_components;
    // to be able to pass component's entity while iterating over them without jumping around the memory 
    std::vector<EntityID> m_entities;
    // Maps an entity to its position in the dense arrays.
    std::unordered_map<EntityID, size_t> m_entityToIndex;
};