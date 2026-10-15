#pragma once

#include <memory>
#include <type_traits>

#include "ResourceManager/ResourceBase.h"

template <typename TResource>
class ResourceHandle
{ 
    static_assert(std::is_base_of_v<ResourceBase, TResource>, "TResource must derive from ResourceBase");

public:
    
    ResourceHandle() = default;
    
    ResourceHandle(const ResourceHandle& other) = default;
    ResourceHandle& operator=(const ResourceHandle& other) = default;
    
    ResourceHandle(ResourceHandle&& other) noexcept = default;
    ResourceHandle& operator=(ResourceHandle&& other) noexcept = default;
    
    ~ResourceHandle() = default;
    
    bool IsValid() const
    {
        return m_resourcePtr != nullptr;
    }
    
    const TResource* Get() const
    {
        return m_resourcePtr.get();
    }
    
    TResource* Get()
    {
        return m_resourcePtr.get();
    }
    
protected:
    
    template <typename T>
    friend class ResourceContainer;
    
    std::shared_ptr<TResource> m_resourcePtr;
    
};
