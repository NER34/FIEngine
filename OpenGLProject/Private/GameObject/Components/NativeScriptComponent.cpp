#include "GameObject/Components/NativeScriptComponent.h"

#include <utility>

#include "GameObject/ComponentAllocator.h"
#include "GameObject/ComponentFactory.h"
#include "GameObject/ScriptRegistry.h"
#include "GameObject/ScriptFactory.h"
#include "Utilities/Logger.h"

NativeScriptComponent::NativeScriptComponent(const NativeScriptComponent& other)
    : m_scriptFactory(other.m_scriptFactory)
{}

NativeScriptComponent& NativeScriptComponent::operator=(const NativeScriptComponent& other)
{
    if (this == &other)
        return *this;
    DestroyScript();
    m_beginPlay = nullptr;
    m_update = nullptr;
    m_endPlay = nullptr;
    m_deserialize = nullptr;
    m_copy = nullptr;
    m_scriptFactory = other.m_scriptFactory;
    return *this;
}

NativeScriptComponent::NativeScriptComponent(NativeScriptComponent&& other) noexcept
    : m_beginPlay(std::move(other.m_beginPlay))
    , m_update(std::move(other.m_update))
    , m_endPlay(std::move(other.m_endPlay))
    , m_deserialize(std::move(other.m_deserialize))
    , m_copy(std::move(other.m_copy))
    , m_scriptInst(std::exchange(other.m_scriptInst, nullptr))
    , m_scriptFactory(std::exchange(other.m_scriptFactory, nullptr))
{}

NativeScriptComponent& NativeScriptComponent::operator=(NativeScriptComponent&& other) noexcept
{
    if (this == &other)
        return *this;
    DestroyScript();
    m_beginPlay = std::move(other.m_beginPlay);
    m_update = std::move(other.m_update);
    m_endPlay = std::move(other.m_endPlay);
    m_deserialize = std::move(other.m_deserialize);
    m_copy = std::move(other.m_copy);
    m_scriptInst = std::exchange(other.m_scriptInst, nullptr);
    m_scriptFactory = std::exchange(other.m_scriptFactory, nullptr);
    return *this;
}

NativeScriptComponent::~NativeScriptComponent()
{
    DestroyScript();
}

void NativeScriptComponent::DestroyScript()
{
    delete m_scriptInst;
    m_scriptInst = nullptr;
}

void NativeScriptComponent::BeginPlay()
{
    if (m_scriptInst)
        m_beginPlay(m_scriptInst);
}

void NativeScriptComponent::Update(float deltaTime)
{
    if (m_scriptInst)
        m_update(m_scriptInst, deltaTime);
}

void NativeScriptComponent::EndPlay()
{
    if (m_scriptInst)
        m_endPlay(m_scriptInst);
}

ScriptBase* NativeScriptComponent::GetScript() const
{
    return m_scriptInst;
}

void NativeScriptComponent::Deserialize(
    NativeScriptComponent& component, 
    const nlohmann::json& data,
    const std::unordered_map<uint32_t, entt::entity>& entityIDMap
    )
{
    if (!data.contains("script") || !data["script"].is_string())
    {
        LOG_ERROR("Native script component must contain a script name");
        return;
    }

    const std::string scriptName = data["script"];
    if (!ScriptRegistry::Get().IsValidScriptName(scriptName))
    {
        LOG_ERROR("Native script component references unknown script {}", scriptName);
        return;
    }

    if (!data.contains("data") || !data["data"].is_object())
    {
        LOG_ERROR("Native script component data is invalid: {}", scriptName);
        return;
    }

    ScriptRegistry::Get().CreateScript(scriptName, component);
    component.m_deserialize(component.m_scriptInst, data["data"], entityIDMap);
    //component.Bind(scriptName, data.contains("data") ? data["data"] : nlohmann::json::object());
}

void NativeScriptComponent::Copy(
    const NativeScriptComponent& from, 
    NativeScriptComponent& to,
    const std::unordered_map<entt::entity, entt::entity>& entityIDMap
    )
{
    if (from.m_scriptFactory)
    {
        from.m_scriptFactory->Create(to);
        to.m_scriptFactory = from.m_scriptFactory;
        to.m_copy(to.m_scriptInst, from.GetScript(), entityIDMap);
    }
}

REGISTER_COMPONENT_FACTORY(NativeScriptComponent, NativeScriptComponent::Deserialize, NativeScriptComponent::Copy)

