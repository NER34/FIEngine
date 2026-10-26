#include "Scene/SceneManager.h"

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

entt::registry& SceneManager::GetRegistry()
{
    return m_registry;
}
