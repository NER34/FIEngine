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
    
    virtual void FreeAllResources() = 0;
    
};

template <typename TResource>
class ResourceContainer : public IResourceContainer
{
    static_assert(std::is_base_of_v<ResourceBase, TResource>, "TResource must derive from ResourceBase");
    
public:
    
    template<typename... TArgs>
    ResourceHandle<TResource> CreateResource(const std::string& name, TArgs&&... args)
    {
        auto [it, inserted] = m_resources.try_emplace(name, std::make_shared<TResource>());
        if (inserted)
        {
            it->second->m_filePath = name;
        }
        ResourceHandle<TResource> handle;
        handle.m_resourcePtr = it->second;
        return handle;
    }
    
    ResourceHandle<TResource> LoadResource(const std::string& filePath)
    {
        auto [it, inserted] = m_resources.try_emplace(filePath, std::make_shared<TResource>());
        if (inserted)
        {
            it->second->m_filePath = filePath;
            it->second->Reload();
        }
        ResourceHandle<TResource> handle;
        handle.m_resourcePtr = it->second;
        return handle;
    }
    
    void FreeResource(const std::string& filePath)
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
    
    bool GetResource(const std::string& name, ResourceHandle<TResource>& outHandle)
    {
        if (auto it = m_resources.find(name); it != m_resources.end())
        {
            outHandle.Reset();
            outHandle.m_resourcePtr = it->second;
            return true;
        }
        return false;
    }
    
protected:
    
    std::unordered_map<std::string, std::shared_ptr<TResource>> m_resources;
    
};
