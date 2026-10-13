#pragma once

#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "ResourceManager/ResourceBase.h"
#include "ResourceHandle.h"

class IResourceContainer
{
public:
    
    IResourceContainer() = default;
    virtual ~IResourceContainer() = default;

    /*
    template <typename TResource>
    ResourceHandle<TResource> AllocateResource(const std::string& name, const std::string& filePath)
    {
        ResourceHandle<TResource> handle;        
        AllocateResource_Internal(name, filePath, handle);
        return handle;
    }
    
    virtual void AllocateResource_Internal(const std::string& name, const std::string& filePath, ResourceHandleBase& outHandle) = 0;
    virtual void FreeResource(const std::string& name) = 0;
    
    template <typename TResource>
    const TResource* GetResource(const std::string& name) const
    {
        return static_cast<const TResource*>(GetResource(name));
    }
    virtual const ResourceBase* GetResource(const std::string& name) const = 0;
    */
    virtual void FreeAllResources() = 0;
    
};

template <typename TResource>
class ResourceContainer : public IResourceContainer
{
    static_assert(std::is_base_of_v<ResourceBase, TResource>, "TResource must derive from ResourceBase");
    
public:
    
    //void AllocateResource_Internal(const std::string& name, const std::string& filePath, ResourceHandleBase& outHandle) //override
    ResourceHandle<TResource> LoadResource(const std::string& filePath/*, ResourceHandleBase& outHandle*/) //override
    {
        auto [it, inserted] = m_resources.try_emplace(filePath, std::make_shared<TResource>());
        if (inserted)
        {
            it->second->m_filePath = filePath;
            //it->second->SetName(name);
            it->second->Reload();
        }
        ResourceHandle<TResource> handle;
        handle.m_resourcePtr = it->second;
        return handle;
    }
    
    /*
    ResourceHandle<TResource> GetResource(const std::string& name) const //override
    {
        ResourceHandle<TResource> handle;
        if (auto it = m_resources.find(name); it != m_resources.end())
        {
            handle.m_resourcePtr = it->second;
        }
        return handle;
    }
    */
    
    void FreeResource(const std::string& filePath) //override
    {
        if (auto it = m_resources.find(filePath); it != m_resources.end())
        {
            it->second->Free();
            m_resources.erase(it);
        }
    }
    
    void FreeAllResources() override
    {
        for (auto& resource : m_resources | std::views::values)
        {
            resource->Free();
        }
        m_resources.clear();
    }
    
protected:
    
    std::unordered_map<std::string, std::shared_ptr<TResource>> m_resources;
    
};
