#include "GameObject/Objects/PlainGameObject.h"

#include "Rendering/Renderer.h"
#include "ResourceManager/ResourceManager.h"

namespace
{
    const char* materialPath = "Materials/Default.asset";

    // Define vertices
    const GLfloat vertices[] = {
        0.0f, 0.5f, 0.5f, // top left
        0.0f, -0.5f, 0.5f, // top right
        0.0f, 0.5f, -0.5f, // bot left
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
    ResourceManager& resourceManager = ResourceManager::Get();
    m_materialResourceHandle = resourceManager.LoadResource<MaterialResource>(materialPath);

    MeshDesc meshDesc;
    meshDesc.m_numVerts = sizeof(vertices) / (sizeof(GLfloat) * 3);
    meshDesc.m_vertices.assign(vertices, vertices + meshDesc.m_numVerts * 3);
    meshDesc.m_uvCoords.assign(uvs, uvs + meshDesc.m_numVerts * 2);
    meshDesc.m_normals.resize(meshDesc.m_numVerts * 3, 0.0f);
    meshDesc.m_indices.assign(indices, indices + sizeof(indices) / sizeof(GLuint));
    m_mesh.setup(meshDesc);

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
    
	MaterialResource* materialResource = m_materialResourceHandle.Get();

    Renderer& renderer = Renderer::get();
    renderer.addDraw(GeometryDrawDesc(m_mesh, *materialResource->GetMaterial(), m_transform));
}
