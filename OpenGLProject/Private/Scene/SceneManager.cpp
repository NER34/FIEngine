#include "Scene/SceneManager.h"

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

SceneManager::~SceneManager()
{
    Free();
}

void SceneManager::InstantiateScene(const ResourceHandle<SceneResource>& scene)
{
    scene->CopyToRegistry(m_registry);
}

void SceneManager::Free()
{
    m_registry.clear();
}
