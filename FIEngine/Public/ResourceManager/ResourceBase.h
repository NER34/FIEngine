#pragma once

#include <memory>
#include <string>

class ResourceBase
{
public:
    
    ResourceBase() = default;
    virtual ~ResourceBase() = default;
    
    virtual void Reload() {}
    virtual void Free() {}
    
};
