#pragma once

#include <memory>
#include <type_traits>

#include "ResourceManager/ResourceBase.h"

template <typename TResource>
class ResourceHandle
{ 
    static_assert(std::is_base_of_v<ResourceBase, TResource>, "TResource must derive from ResourceBase");

public:
    
    bool IsValid() const
    {
        return m_resourcePtr != nullptr;
    }
    
    const TResource* Get() const
    {
        return m_resourcePtr.get();
    }
    
protected:
    
    template <typename T>
    friend class ResourceContainer;
    
    std::shared_ptr<TResource> m_resourcePtr;
    
};
