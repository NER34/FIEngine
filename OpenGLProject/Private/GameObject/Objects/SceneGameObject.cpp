#include "GameObject/Objects/SceneGameObject.h"

#include "GameObject/Components/NativeScriptComponent.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"

namespace
{
    const std::string g_scenePath = "Scenes/Default_NoPlane_C&L.asset";
}

void SceneGameObject::BeginPlay()
{
    m_sceneHandle = ResourceManager::Get().LoadResource<SceneResource>(g_scenePath);
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
}
