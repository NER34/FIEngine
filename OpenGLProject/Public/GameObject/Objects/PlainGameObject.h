#pragma once

#include "GameObject/GameObject.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "ResourceManager/Resources/MeshResource.h"

class PlainGameObject : public GameObject
{
public:
    
    void BeginPlay() override;
    void Update(float deltaTime) override;
    
private:
    
    Transform m_transform;
    float m_globalVal = 0.0f;
    
    ResourceHandle<MeshResource> m_meshResourceHandle;
    ResourceHandle<MaterialResource> m_materialResourceHandle;
    
};
