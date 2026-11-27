#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>

#include "ResourceContainer.h"
#include "ResourceHandle.h"
#include "SystemBase/SystemLocator.h"

class ResourceManager : public ISystemBase
{
public:
    
    ResourceManager() = default;
    ~ResourceManager();
    
    void Deinitialize() override;

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
    void FreeResource(const std::string& filePath)
    {
        if (auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            auto* container = static_cast<ResourceContainer<TResource>*>(it->second.get());
            container->FreeResource(filePath);
        }
    }
    
    template<typename TResource>
    bool GetResource(const std::string& name, ResourceHandle<TResource>& outHandle)
    {
        if (const auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            auto* container = static_cast<ResourceContainer<TResource>*>(it->second.get());
            return container->GetResource(name, outHandle);
        }
        return false;
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
