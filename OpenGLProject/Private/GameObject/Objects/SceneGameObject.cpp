#include "GameObject/Objects/SceneGameObject.h"

#include "ResourceManager/ResourceManager.h"

namespace
{
    const std::string g_scenePath = "Scenes/Default_NoPlane.asset";
}

void SceneGameObject::BeginPlay()
{
    m_sceneHandle = ResourceManager::Get().LoadResource<SceneResource>(g_scenePath);
}
