#include "Scripting/ScriptManager.h"

#include "Components/NativeScriptComponent.h"
#include "Scene/SceneManager.h"

ScriptManager& ScriptManager::Get()
{
    static ScriptManager manager;
    return manager;
}

void ScriptManager::Initialize()
{
    BeginPlayPendingScripts();
}

void ScriptManager::Update(float deltaTime)
{
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
