#pragma once

#include <memory>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include "Utilities/Logger.h"

template <typename TKey, typename TObject>
class Registry
{
public:
    
    template <typename TDerivedObject, typename... TArgs>
    bool RegisterObject(const TKey& key, TArgs&&... args)
    {
        if (!m_registry.contains(key))
        {
            m_registry.insert({ key, std::make_unique<TDerivedObject>(std::forward<TArgs>(args)...) });
            return true;
        }
        // Non-string keys (e.g. SystemLocator's type_index) are reported by the owning class
        if constexpr (std::is_convertible_v<const TKey&, std::string_view>)
        {
            LOG_ERROR("Failed to register object: key {} already registered", std::string_view(key));
        }
        return false;
    }
    
    TObject* GetObject(const TKey& key) const
    {
        if (auto it = m_registry.find(key); m_registry.end() != it)
        {
            return it->second.get();
        }
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
