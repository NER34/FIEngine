#include "GameObject/Objects/PlainGameObject.h"

#include "Rendering/Renderer.h"
#include "Rendering/Mesh/MeshManager.h"

namespace
{
	const char* vertShaderPath = "Shaders/VertexShader.vert";
	const char* fragShaderPath = "Shaders/FragmentShader.frag";
	const std::string meshName = "plain";
	
	// Define vertices
	const GLfloat vertices[] = {
		0.0f,  0.5f,  0.5f, // top left
		0.0f, -0.5f,  0.5f, // top right
		0.0f,  0.5f, -0.5f, // bot left
		0.0f, -0.5f, -0.5f, // bot right
	};	
	
	const GLfloat uvs[] = {
		0.0f, 0.0f, // top left
		1.0f, 0.0f, // top right
		0.0f, 1.0f, // bot left
		1.0f, 1.0f, // bot right	
	};
		
	const GLuint indices[] = {
		2, 1, 0,
		2, 3, 1
	};
}

void PlainGameObject::beginPlay()
{
    auto resourcesPath = getResourcesPath();
		
    MaterialDesc materialDesc;
    materialDesc.m_shadersDesc = {
			    { GL_VERTEX_SHADER, (resourcesPath / vertShaderPath).string() },
                { GL_FRAGMENT_SHADER, (resourcesPath / fragShaderPath).string() }
    };
    materialDesc.m_texturesDesc = {
			    { "SolidColor", "Textures/test_image.png" }
    };
    m_material.setup(materialDesc);
		
    MeshDesc meshDesc;
	meshDesc.m_numVerts = sizeof(vertices) / (sizeof(GLfloat) * 3);
	meshDesc.m_vertices.assign(vertices, vertices + meshDesc.m_numVerts * 3);
	meshDesc.m_uvCoords.assign(uvs, uvs + meshDesc.m_numVerts * 2);
	meshDesc.m_normals.resize(meshDesc.m_numVerts * 3, 0.0f);
    meshDesc.m_indices.assign(indices, indices + sizeof(indices) / sizeof(GLuint));
	m_mesh.setup(meshDesc);
	/*
	MeshManager& meshManager = MeshManager::get();
	meshManager.registerMesh(meshName, meshDesc);
	MeshManager::ConstMeshIterator begIt, endIt;
	meshManager.getMeshes(meshName, begIt, endIt);
	m_mesh = begIt._Ptr;
	*/
	
    m_transform.m_position = glm::vec3(2.0f, 0.0f, 0.0f);
    m_transform.m_rotation = glm::vec3(0.0f, 0.0f, 180.0f);
}

void PlainGameObject::update(float deltaTime)
{
	m_globalVal += deltaTime;
	
	auto& rotation = m_transform.m_rotation;
	rotation.x = 10 * m_globalVal;
			
	auto& scale = m_transform.m_scale;
	scale = glm::vec3(1.0f) * (sin(m_globalVal) / 2.0f + 1.0f);
			
	auto& position = m_transform.m_position;
	position.z = sin(m_globalVal) / 2.0f;
			
	Renderer& renderer = Renderer::get();
	renderer.addDraw(GeometryDrawDesc(m_mesh, m_material, m_transform));
}
