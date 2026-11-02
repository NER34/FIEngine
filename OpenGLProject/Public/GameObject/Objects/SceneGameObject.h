#pragma once

#include "GameObject/GameObject.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/SceneResource.h"

class SceneGameObject : public GameObject
{
public:
    
    void BeginPlay() override;
    void EndPlay() override;
    
private:
    
    ResourceHandle<SceneResource> m_sceneHandle;
    
};
