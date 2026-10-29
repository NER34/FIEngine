#include "GameObject/Objects/SceneGameObject.h"

#include "AssetManager/AssetManager.h"
#include "Components/NativeScriptComponent.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Utilities/Logger.h"

namespace
{
    const std::string g_scenePath = "Scenes/Default_NoPlane_C&L.asset";
}

void SceneGameObject::BeginPlay()
{
    if (!AssetManager::Get().Load<SceneResource>(g_scenePath, m_sceneHandle))
    {
        LOG_ERROR("Failed to load scene {}", g_scenePath);
        return;
    }
    SceneManager::Get().InstantiateScene(m_sceneHandle);
    
    SceneManager::Get().View<NativeScriptComponent>().each(
        [&](auto entity, NativeScriptComponent& component)
        {
            component.BeginPlay();
        });
}

void SceneGameObject::Update(float deltaTime)
{
    SceneManager::Get().View<NativeScriptComponent>().each(
        [&](auto entity, NativeScriptComponent& component)
        {
            component.Update(deltaTime);
        });
}

void SceneGameObject::EndPlay()
{
    SceneManager::Get().View<NativeScriptComponent>().each(
        [&](auto entity, NativeScriptComponent& component)
        {
            component.EndPlay();
        });
    
    m_sceneHandle.Reset();
}
