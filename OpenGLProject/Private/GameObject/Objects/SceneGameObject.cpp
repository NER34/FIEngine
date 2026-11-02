#include "GameObject/Objects/SceneGameObject.h"

#include "AssetManager/AssetManager.h"
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
    SceneManager::Get().SpawnScene(m_sceneHandle);
}

void SceneGameObject::EndPlay()
{
    m_sceneHandle.Reset();
}
