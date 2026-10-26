#pragma once

/*
#include <json.h>

#include "ComponentAllocator.h"
#include "ComponentFactory.h"

class IComponentBase
{
public:
    virtual ~IComponentBase() = default;
    virtual void Deserialize(const nlohmann::json& data) = 0;     
};

template <typename TDerivedComponent>
class ComponentBase : public IComponentBase
{
public:

    ComponentBase() { s_registered; }
    virtual ~ComponentBase() = default;

private:
    
    static bool s_registered;
    static bool RegisterFactory()
    {
        using TFactory = ComponentFactory<TDerivedComponent>;
        ComponentAllocator::Get().RegisterComponentFactory<TFactory>(typeid(TDerivedComponent).name());
        return true;
    }
    
};

template<typename TComponent>
bool ComponentBase<TComponent>::s_registered = ComponentBase<TComponent>::RegisterFactory();
*/
