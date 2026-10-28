#pragma once

#include "ComponentFactory/ComponentFactory.h"

class IScriptFactory;
class ScriptBase;

class NativeScriptComponent
{
public:

private:
    using TBeginPlayFunc = std::function<void(ScriptBase*)>;
    using TUpdateFunc = std::function<void(ScriptBase*, float deltaTime)>;
    using TEndPlayFunc = std::function<void(ScriptBase*)>;
    
public:

    NativeScriptComponent() = default;

    NativeScriptComponent(const NativeScriptComponent&) = delete;
    NativeScriptComponent& operator=(const NativeScriptComponent&) = delete;

    NativeScriptComponent(NativeScriptComponent&& other) noexcept;
    NativeScriptComponent& operator=(NativeScriptComponent&& other) noexcept;

    ~NativeScriptComponent();

    template <typename TScript>
    TScript* Bind()
    {
        DestroyScript();
        m_scriptInst = new TScript();
        m_beginPlay = [](ScriptBase* script)
        {
            static_cast<TScript*>(script)->BeginPlay();
        };
        m_update = [](ScriptBase* script, float deltaTime)
        {
            static_cast<TScript*>(script)->Update(deltaTime);
        };
        m_endPlay = [](ScriptBase* script)
        {
            static_cast<TScript*>(script)->EndPlay();
        };
        return static_cast<TScript*>(m_scriptInst);
    }

    void BeginPlay();
    void Update(float deltaTime);
    void EndPlay();

    ScriptBase* GetScript() const;
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, NativeScriptComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, NativeScriptComponent);
    
private:

    void DestroyScript();

    TBeginPlayFunc      m_beginPlay;
    TUpdateFunc         m_update;
    TEndPlayFunc        m_endPlay;
    
    ScriptBase*     m_scriptInst = nullptr;
    IScriptFactory* m_scriptFactory = nullptr;

};
