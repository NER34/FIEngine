#pragma once

#include <memory>
#include <type_traits>

#include "ResourceManager/ResourceBase.h"

template <typename TResource>
class ResourceHandle
{ 
public:
    
    ResourceHandle()
    {
        static_assert(std::is_base_of_v<ResourceBase, TResource>, "TResource must derive from ResourceBase");
    }
    
    ResourceHandle(const ResourceHandle& other) = default;
    ResourceHandle& operator=(const ResourceHandle& other) = default;
    
    ResourceHandle(ResourceHandle&& other) noexcept = default;
    ResourceHandle& operator=(ResourceHandle&& other) noexcept = default;
    
    ~ResourceHandle() = default;
    
    bool IsValid() const
    {
        return m_resourcePtr != nullptr;
    }
    
    const TResource* operator->() const
    {
        return m_resourcePtr.get();
    }
    
    TResource* operator->()
    {
        return m_resourcePtr.get();
    }
    
    auto operator<=>(const ResourceHandle&) const = default;
    
    void Reset()
    {
        m_resourcePtr.reset();
    }
    
protected:
    
    template <typename T>
    friend class ResourceContainer;
    
    std::shared_ptr<TResource> m_resourcePtr;
    
};
