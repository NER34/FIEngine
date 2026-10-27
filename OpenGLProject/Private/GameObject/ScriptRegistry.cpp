#include "GameObject/ScriptRegistry.h"

#include "GameObject/ScriptFactory.h"
#include "GameObject/Components/NativeScriptComponent.h"

ScriptRegistry& ScriptRegistry::Get()
{
    static ScriptRegistry scriptAllocator;
    return scriptAllocator;
}

void ScriptRegistry::CreateScript(const std::string& name, NativeScriptComponent& component) const
{
    if (auto it = m_scriptFactories.find(name); it != m_scriptFactories.end())
    {
        it->second->Create(component);
        component.m_scriptFactory = it->second.get();
        return;
    }
    LOG_ERROR("Unknown script {}", name);
}

bool ScriptRegistry::IsValidScriptName(const std::string& name) const
{
    return m_scriptFactories.contains(name);
}
