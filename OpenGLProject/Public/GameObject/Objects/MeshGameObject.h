#pragma once

#include "GameObject/GameObject.h"
#include "Rendering/Material/Material.h"
#include "Rendering/Mesh/Mesh.h"

class MeshGameObject : public GameObject
{
public:
    
    void beginPlay() override;
    void update(float deltaTime) override;
    
private:
    
    Mesh m_mesh;
    Material m_material; 
    Transform m_transform;
    float m_globalVal = 0.0f;
    
};
