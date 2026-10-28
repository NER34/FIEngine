#include "Components/NativeScriptComponent.h"

#include <utility>

#include "Scripting/ScriptBase.h"
#include "Scripting/ScriptFactory.h"
#include "Utilities/Logger.h"

NativeScriptComponent::NativeScriptComponent(NativeScriptComponent&& other) noexcept
    : m_beginPlay(std::move(other.m_beginPlay))
    , m_update(std::move(other.m_update))
    , m_endPlay(std::move(other.m_endPlay))
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

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, NativeScriptComponent)
{
    if (!data.contains("script") || !data["script"].is_string())
    {
        LOG_ERROR("Native script component must contain a script name");
        return;
    }
    
    const std::string scriptName = data["script"];
    IScriptFactory* scriptFactory = ScriptRegistry::Get().GetObject(scriptName);
    if (!scriptFactory)
    {
        LOG_ERROR("Native script component references unknown script {}", scriptName);
        return;
    }

    if (!data.contains("data") || !data["data"].is_object())
    {
        LOG_ERROR("Native script component data is invalid: {}", scriptName);
        return;
    }

    scriptFactory->CreateFromAsset(component, data["data"], mapping);
    component.m_scriptFactory = scriptFactory;
}

DEFINE_COMPONENT_COPY_FUNC(Copy, NativeScriptComponent)
{
    if (from.m_scriptFactory)
    {
        from.m_scriptFactory->CloneScript(from, to, mapping);
        to.m_scriptFactory = from.m_scriptFactory;
    }
}

REGISTER_COMPONENT_FACTORY(NativeScriptComponent, NativeScriptComponent::Deserialize, NativeScriptComponent::Copy)

