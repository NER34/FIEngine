#include "GameObject/Objects/MeshGameObject.h"

#include "Rendering/Renderer.h"
#include "Rendering/Mesh/MeshManager.h"

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
			    { GL_VERTEX_SHADER, (resourcesPath / vertShaderPath).string() },
                { GL_FRAGMENT_SHADER, (resourcesPath / fragShaderPath).string() }
    };
    materialDesc.m_texturesDesc = {
			    { "SolidColor", { (resourcesPath / meshTexturePath).string() } }
    };
    m_material.setup(materialDesc);
		
    MeshLoadDesc meshLoadDesc;
	meshLoadDesc.m_meshName = meshName;
	meshLoadDesc.m_meshPath = meshPath;
	
	MeshManager& meshManager = MeshManager::get();
	meshManager.loadMesh(meshLoadDesc);
	/*
	meshManager.registerMesh(meshName, meshDesc);
	MeshManager::ConstMeshIterator begIt, endIt;
	meshManager.getMeshes(meshName, begIt, endIt);
	m_mesh = begIt._Ptr;
	*/
	
    m_transform.m_position = glm::vec3(2.0f, 2.0f, 0.0f);
    m_transform.m_rotation = glm::vec3(0.0f, 0.0f, 180.0f);
}

void MeshGameObject::update(float deltaTime)
{
	m_globalVal += deltaTime;

	MeshManager& meshManager = MeshManager::get();
	MeshManager::ConstMeshIterator begIt, endIt;
	meshManager.getMeshes(meshName, begIt, endIt);
			
	Renderer& renderer = Renderer::get();
	for (auto it = begIt; it != endIt; ++it)
	{
		renderer.addDraw(GeometryDrawDesc(*it._Ptr, m_material, m_transform));
	}
}
