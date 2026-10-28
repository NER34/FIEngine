#pragma once

#include <memory>
#include <unordered_map>

#include "Utilities/Logger.h"

template <typename TKey, typename TObject>
class Registry
{
public:
    
    template <typename TDerivedObject, typename... TArgs>
    bool FindObject(const TKey& key, TArgs&&... args)
    {
        if (!m_registry.contains(key))
        {
            m_registry.insert({ key, std::make_unique<TDerivedObject>(std::forward<TArgs>(args)...) });
            return true;
        }
        LOG_ERROR("Failed to register object: key {} already registered", key);
        return false;
    }
    
    TObject* GetObject(const TKey& key) const
    {
        if (auto it = m_registry.find(key); m_registry.end() != it)
        {
            return it->second.get();
        }
        LOG_ERROR("Failed to get object: key {} is not registered", key);
        return nullptr;
    }

    template <typename TFunc>
    void ForEachObject(TFunc&& func) const
    {
        for (const auto& [key, object] : m_registry)
        {
            func(key, *object);
        }
    }

private:
    
    std::unordered_map<TKey, std::unique_ptr<TObject>> m_registry;
    
};
