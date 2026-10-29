#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>

#include "ResourceContainer.h"
#include "ResourceHandle.h"

class ResourceManager
{
public:
    
    ResourceManager() = default;
    ~ResourceManager();

    static ResourceManager& Get();
    
    void FreeAllResources();
    
    template<typename TResource, typename... TArgs>
    ResourceHandle<TResource> CreateResource(const std::string& name, TArgs&&... args)
    {
        if (!m_registeredContainers.contains(typeid(TResource)))
        {
            RegisterResourceType<ResourceContainer<TResource>, TResource>();
        }
        auto it = m_registeredContainers.find(typeid(TResource));
        auto* container = static_cast<ResourceContainer<TResource>*>(it->second.get());
        return container->CreateResource(name, std::forward<TArgs>(args)...);
    }
    
    template<typename TResource>
    ResourceHandle<TResource> LoadResource(const std::string& filePath)
    {
        if (!m_registeredContainers.contains(typeid(TResource)))
        {
            RegisterResourceType<ResourceContainer<TResource>, TResource>();
        }
        auto it = m_registeredContainers.find(typeid(TResource));
        auto* container = static_cast<ResourceContainer<TResource>*>(it->second.get());
        return container->LoadResource(filePath);
    }
    
    template<typename TResource>
    void FreeResource(const std::string& filePath)
    {
        if (auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            auto* container = static_cast<ResourceContainer<TResource>*>(it->second);
            container->FreeResource(filePath);
        }
    }
    
private:
    
    template <typename TResourceContainer, typename TResource>
    void RegisterResourceType()
    {        
        const std::type_index index = typeid(TResource);
        m_registeredContainers.insert({ index, std::make_unique<TResourceContainer>() });
    }
    
    std::unordered_map<std::type_index, std::unique_ptr<IResourceContainer>> m_registeredContainers;
    
};
