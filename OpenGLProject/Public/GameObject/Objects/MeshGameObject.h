#pragma once

#include "GameObject/GameObject.h"
#include "Rendering/Material/Material.h"
#include "Rendering/Mesh/Mesh.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "ResourceManager/Resources/MeshResource.h"
#include "ResourceManager/Resources/ShaderProgramResource.h"
#include "ResourceManager/Resources/TextureResource.h"

class MeshGameObject : public GameObject
{
public:
    
    void beginPlay() override;
    void update(float deltaTime) override;
    
private:
    
    Material m_material; 
    Transform m_transform;
    float m_globalVal = 0.0f;
    
    ResourceHandle<MeshResource> m_meshResourceHandle;
    
    ResourceHandle<MaterialResource> m_materialResourceHandle;
    /*
    ResourceHandle<ShaderProgramResource> m_shaderProgResHandle;
    ResourceHandle<TextureResource> m_textureResourceHandle;
    */

};
