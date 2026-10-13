#include "GameObject/Objects/MeshGameObject.h"

#include "Rendering/Renderer.h"
#include "Rendering/Mesh/MeshManager.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/Resources/MeshResource.h"

namespace
{
	const char* vertShaderPath = "Shaders/VertexShader.vert";
	const char* fragShaderPath = "Shaders/PhongShader.frag";
	
	const char* meshPath = "Meshes/Sting-Sword-lowpoly.obj";
	const char* meshTexturePath = "Textures/Sting/Sting_Base_Color.png";
	const std::string meshName = "sword";
}

void MeshGameObject::beginPlay()
{
    auto resourcesPath = getResourcesPath();
		
    MaterialDesc materialDesc;
    materialDesc.m_shadersDesc = {
		{ GL_VERTEX_SHADER, (getResourcesPath() / vertShaderPath).string() },
		{ GL_FRAGMENT_SHADER, (getResourcesPath() / fragShaderPath).string() }
    };
    materialDesc.m_texturesDesc = {
		{ "SolidColor", meshTexturePath }
    };
    m_material.setup(materialDesc);
		
    MeshLoadDesc meshLoadDesc;
	meshLoadDesc.m_meshName = meshName;
	meshLoadDesc.m_meshPath = meshPath;
	
	ResourceManager& resourceManager = ResourceManager::Get();
	m_meshResourceHandle = resourceManager.LoadResource<MeshResource>(/*meshName, */meshPath);
	
    m_transform.m_position = glm::vec3(2.0f, 2.0f, 0.0f);
    m_transform.m_rotation = glm::vec3(0.0f, 0.0f, 180.0f);
}

void MeshGameObject::update(float deltaTime)
{
	m_globalVal += deltaTime;

	/*
	MeshManager& meshManager = MeshManager::get();
	MeshManager::ConstMeshIterator begIt, endIt;
	meshManager.getMeshes(meshName, begIt, endIt);
	*/
	
	auto* meshResource = m_meshResourceHandle.Get();
			
	Renderer& renderer = Renderer::get();
	for (const auto& mesh : meshResource->GetMeshes())
	{
		renderer.addDraw(GeometryDrawDesc(mesh, m_material, m_transform));
	}
}
