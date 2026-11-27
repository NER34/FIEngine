#pragma once

#include <functional>

#include "json.h"

#include "Registry/Registry.h"
#include "ComponentFactory/ComponentFactoryStructs.h"
#include "Components/NativeScriptComponent.h"
#include "Utilities/Logger.h"

class IScriptFactory;

class ScriptRegistry : public Registry<std::string, IScriptFactory>
{
public:
    static ScriptRegistry& Get()
    {
        static ScriptRegistry s_instance;
        return s_instance;
    }
};

class IScriptFactory
{
public:
    virtual ~IScriptFactory() = default;
    
    virtual void CreateFromAsset(
        NativeScriptComponent& component, 
        const nlohmann::json& data, 
        const EntityIDMapping& mapping
        ) const = 0;
    
    virtual void CloneScript(
        const NativeScriptComponent& from, 
        NativeScriptComponent& to, 
        const EntityIDMapping& mapping
        ) const = 0;
};

template <typename TScript>
class ScriptFactory : public IScriptFactory
{
    using DeserializeFunc = std::function<void(TScript&, const nlohmann::json&, const EntityIDMapping&)>;
    using CopyFunc = std::function<void(const TScript&, TScript&, const EntityIDMapping&)>;

public:
    
    ScriptFactory(DeserializeFunc deserialize, CopyFunc copy) 
        : m_deserialize(deserialize) 
        , m_copy(copy)
    {}

    void CreateFromAsset(
        NativeScriptComponent& component, 
        const nlohmann::json& data, 
        const EntityIDMapping& mapping
        ) const final
    {
        TScript* script = component.Bind<TScript>();
        m_deserialize(*script, data, mapping);
    }
    
    void CloneScript(
        const NativeScriptComponent& from, 
        NativeScriptComponent& to, 
        const EntityIDMapping& mapping
        ) const final
    {
        const TScript* script_from = static_cast<const TScript*>(from.GetScript());
        if (!script_from)
        {
            LOG_ERROR("Failed to clone script: template script component has invalid script");
            return;
        }
        TScript* script_to = to.Bind<TScript>();
        m_copy(*script_from, *script_to, mapping);
    }

private:

    DeserializeFunc m_deserialize;  
    CopyFunc        m_copy;  
    static bool     s_registered;

};


#define DECLARE_SCRIPT_DESERIALIZE_FUNC(FuncName, TScript) \
    static void FuncName(TScript& script, const nlohmann::json& data, const EntityIDMapping& mapping)

#define DEFINE_SCRIPT_DESERIALIZE_FUNC(FuncName, TScript) \
    void TScript::FuncName(TScript& script, const nlohmann::json& data, const EntityIDMapping& mapping)

#define DECLARE_SCRIPT_COPY_FUNC(FuncName, TScript) \
    static void FuncName(const TScript& from, TScript& to, const EntityIDMapping& mapping)
    
#define DEFINE_SCRIPT_COPY_FUNC(FuncName, TScript) \
    void TScript::FuncName(const TScript& from, TScript& to, const EntityIDMapping& mapping)


#define SCRIPT_FACTORY_REGISTER_VAR(TScript) template<> bool ScriptFactory<TScript>::s_registered
#define SCRIPT_FACTORY_REGISTER_FUNC(TScript) ScriptRegistry::Get().RegisterObject<ScriptFactory<TScript>>

#define REGISTER_SCRIPT_FACTORY(TScript, DeserializeFunc, CopyFunc) \
    SCRIPT_FACTORY_REGISTER_VAR(TScript) = SCRIPT_FACTORY_REGISTER_FUNC(TScript) \
    ( \
        #TScript, DeserializeFunc, CopyFunc \
    );
