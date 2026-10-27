#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Components/NativeScriptComponent.h"
#include "GameObject/ScriptFactory.h"
#include "Utilities/Logger.h"

class IScriptFactory;
class ScriptBase;

class ScriptRegistry
{
public:

    static ScriptRegistry& Get();

    void CreateScript(const std::string& name, NativeScriptComponent& component) const;

    bool IsValidScriptName(const std::string& name) const;

    template<typename TFactory>
    bool RegisterScriptFactory(const std::string& name, TFactory&& factory)
    {
        if (m_scriptFactories.contains(name))
        {
            LOG_ERROR("Script factory with name {} already registered", name);
            return false;
        }
        m_scriptFactories.insert({ name, std::make_unique<TFactory>(std::move(factory)) });
        return true;
    }

private:

    std::unordered_map<std::string, std::unique_ptr<IScriptFactory>> m_scriptFactories;
};

#define REGISTER_SCRIPT_FACTORY(TScript) \
    template<> bool ScriptFactory<TScript>::s_registered = ScriptRegistry::Get().RegisterScriptFactory( \
        #TScript, ScriptFactory<TScript>() \
    );
