#pragma once

#include <type_traits>

#include "Components/NativeScriptComponent.h"
#include "GameObject/ScriptBase.h"

class IScriptFactory
{
public:
    virtual ~IScriptFactory() = default;
    virtual void Create(NativeScriptComponent& component) const = 0;
};

template <typename TScript>
class ScriptFactory : public IScriptFactory
{
    static_assert(std::is_base_of_v<ScriptBase, TScript>, "TScript must derive from ScriptBase");

public:

    void Create(NativeScriptComponent& component) const final
    {
        component.Bind<TScript>();
    }

private:

    static bool s_registered;

};
