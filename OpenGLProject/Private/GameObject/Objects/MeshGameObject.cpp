#include "GameObject/Objects/MeshGameObject.h"

#include "Rendering/Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/MeshResource.h"

namespace
{
	const char* meshPath = "Meshes/Sting-Sword-lowpoly.obj";
	const char* materialPath = "Materials/Phong.asset";
}

void MeshGameObject::BeginPlay()
{		
	ResourceManager& resourceManager = ResourceManager::Get();
	
	m_meshResourceHandle = resourceManager.LoadResource<MeshResource>(meshPath);
	m_materialResourceHandle = resourceManager.LoadResource<MaterialResource>(materialPath);
	
    m_transform.m_position = glm::vec3(2.0f, 2.0f, 0.0f);
    m_transform.m_rotation = glm::vec3(0.0f, 0.0f, 180.0f);
}

void MeshGameObject::Update(float deltaTime)
{
	m_globalVal += deltaTime;

	MeshResource* meshResource = m_meshResourceHandle.Get();
	MaterialResource* materialResource = m_materialResourceHandle.Get();
	
	Renderer& renderer = Renderer::get();
	for (auto& mesh : meshResource->GetMeshes())
	{
		renderer.addDraw(GeometryDrawDesc(mesh, *materialResource->GetMaterial(), m_transform));
	}
}
