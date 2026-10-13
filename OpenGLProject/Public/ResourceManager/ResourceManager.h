#pragma once
#include <string>
#include <typeindex>
#include <unordered_map>

#include "ResourceContainer.h"
#include "ResourceHandle.h"

class IResourceContainer;

class ResourceManager
{
public:
    
    ResourceManager() = default;
    ~ResourceManager();
    
    static ResourceManager& Get();
    
    void Initialize();
    void FreeAllResources();
    
    template<typename TResource>
    ResourceHandle<TResource> LoadResource(const std::string& name, const std::string& fileName)
    {
        if (auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            auto* container = static_cast<ResourceContainer<TResource>*>(it->second);
            return container->LoadResource(name, fileName);
        }
        return ResourceHandle<TResource>();
    }
    
    /*
    template<typename TResource>
    const TResource* GetResource(const std::string& name) const
    {
        if (auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            return it->second->GetResource<TResource>(name);
        }
        return nullptr;
    }
    */
    
    template<typename TResource>
    void FreeResource(const std::string& name)
    {
        if (auto it = m_registeredContainers.find(typeid(TResource)); it != m_registeredContainers.end())
        {
            auto* container = static_cast<ResourceContainer<TResource>*>(it->second);
            container->FreeResource(name);
        }
    }
    
    /*
    void LoadResource_Internal(const std::string& name, const std::string& fileName, IResourceContainer* container);
    void FreeResource_Internal(const std::string& name, IResourceContainer* container);
    */
    
private:
    
    template <typename TResourceContainer, typename TResource>
    void RegisterResourceType()
    {        
        const std::type_index index = typeid(TResource);
        m_registeredContainers.insert({ index, new TResourceContainer() });
    }
    
    std::unordered_map<std::type_index, IResourceContainer*> m_registeredContainers;
    
};
