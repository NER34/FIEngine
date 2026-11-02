#include "Scripts/SpawnScene.h"

#include "AssetManager/AssetManager.h"
#include "Scene/SceneManager.h"

void SpawnScene::BeginPlay()
{
    if (m_scene.IsValid())
    {
        SceneManager::Get().SpawnScene(m_scene);
    }
}

DEFINE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SpawnScene)
{
    if (!data.contains("scene") || !data["scene"].is_string())
    {
        LOG_ERROR("Spawn Scene script must contain a scene path");
    }
    else
    {
        const std::string& scenePath = data["scene"];
        if (!AssetManager::Get().Load<SceneResource>(scenePath, script.m_scene))
        {
            LOG_ERROR("Spawn Scene script failed to load scene {}", scenePath);
        }
    }
}

DEFINE_SCRIPT_COPY_FUNC(Copy, SpawnScene)
{
    to.m_scene = from.m_scene;
}

REGISTER_SCRIPT_FACTORY(SpawnScene, SpawnScene::Deserialize, SpawnScene::Copy)
