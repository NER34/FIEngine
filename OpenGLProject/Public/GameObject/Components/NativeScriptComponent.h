#pragma once

#include <memory>

#include "entt.h"
#include "json.h"

#include "GameObject/ComponentAllocator.h"

class IScriptFactory;

class ScriptBase;

class NativeScriptComponent
{
public:

private:
    using TBeginPlayFunc = std::function<void(ScriptBase*)>;
    using TUpdateFunc = std::function<void(ScriptBase*, float deltaTime)>;
    using TEndPlayFunc = std::function<void(ScriptBase*)>;
    using TDeserializeFunc = std::function<void(
        ScriptBase*, const nlohmann::json&, const std::unordered_map<uint32_t, entt::entity>&
        )>;
    using TCopyFunc = std::function<void(
        ScriptBase*, ScriptBase*, const std::unordered_map<entt::entity, entt::entity>&
        )>;
    
public:
    
    NativeScriptComponent() = default;

    NativeScriptComponent(const NativeScriptComponent& other);
    NativeScriptComponent& operator=(const NativeScriptComponent& other);
    
    NativeScriptComponent(NativeScriptComponent&& other) noexcept;
    NativeScriptComponent& operator=(NativeScriptComponent&& other) noexcept;
    
    ~NativeScriptComponent();

    template <typename TScript>
    void Bind()
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
        m_deserialize = [](ScriptBase* script, const nlohmann::json& data, const std::unordered_map<uint32_t, entt::entity>& entityIDMap)
        {
            static_cast<TScript*>(script)->Deserialize(data, entityIDMap);
        };
        m_copy = [](ScriptBase* script, ScriptBase* other, const std::unordered_map<entt::entity, entt::entity>& entityIDMap)
        {
            static_cast<TScript*>(script)->Copy(other, entityIDMap);
        };
    }

    void BeginPlay();
    void Update(float deltaTime);
    void EndPlay();

    ScriptBase* GetScript() const;
    
    static IMPLEMENT_COMPONENT_DESERIALIZE(Deserialize, NativeScriptComponent);
    static IMPLEMENT_COMPONENT_COPY(Copy, NativeScriptComponent);
    
private:
    
    friend class ScriptRegistry;

    void DestroyScript();

    TBeginPlayFunc      m_beginPlay;
    TUpdateFunc         m_update;
    TEndPlayFunc        m_endPlay;
    TDeserializeFunc    m_deserialize;
    TCopyFunc           m_copy;
    
    ScriptBase*     m_scriptInst = nullptr;
    IScriptFactory* m_scriptFactory = nullptr;

};
