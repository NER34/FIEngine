#include "Scripting/ScriptManager.h"

#include "Components/NativeScriptComponent.h"
#include "Scene/SceneManager.h"
#include "easy/profiler.h"

ScriptManager& ScriptManager::Get()
{
    auto* system = SystemLocator::Get().GetSystem<ScriptManager>();
    assert(system && "ScriptManager is not registered");
    return *static_cast<ScriptManager*>(system);
}

void ScriptManager::Initialize()
{
    EASY_FUNCTION();

    BeginPlayPendingScripts();
}

void ScriptManager::Update(float deltaTime)
{
    EASY_FUNCTION();

    // Scripts spawned since the last frame (e.g. by SpawnScene) begin play before their first update
    BeginPlayPendingScripts();

    SceneManager::Get().View<NativeScriptComponent>().each(
        [&](auto entity, NativeScriptComponent& component)
        {
            component.Update(deltaTime);
        });
}

void ScriptManager::Deinitialize()
{
    EASY_FUNCTION();

    SceneManager::Get().View<NativeScriptComponent>().each(
        [&](auto entity, NativeScriptComponent& component)
        {
            component.EndPlay();
        });
}

void ScriptManager::BeginPlayPendingScripts()
{
    // Components added during iteration aren't visited, so repeat until BeginPlay spawns no new scripts
    bool bHasPending = true;
    while (bHasPending)
    {
        bHasPending = false;
        SceneManager::Get().View<NativeScriptComponent>().each(
            [&](auto entity, NativeScriptComponent& component)
            {
                if (!component.HasBegunPlay())
                {
                    component.BeginPlay();
                    bHasPending = true;
                }
            });
    }
}
