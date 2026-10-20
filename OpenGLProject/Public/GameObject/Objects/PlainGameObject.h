#pragma once

#include "GameObject/GameObject.h"
#include "Rendering/Mesh/Mesh.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MaterialResource.h"

class PlainGameObject : public GameObject
{
public:
    
    void beginPlay() override;
    void update(float deltaTime) override;
    
private:
    
    Mesh m_mesh;
    Transform m_transform;
    float m_globalVal = 0.0f;
    
    ResourceHandle<MaterialResource> m_materialResourceHandle;
    
};
