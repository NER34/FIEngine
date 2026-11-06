#include "Scripts/TestPlane.h"

#include "Components/MeshComponent.h"
#include "Scripting/ScriptFactory.h"
#include "Components/TransformComponent.h"
#include "InputSystem/InputManager.h"
#include "Scene/SceneManager.h"
#include "Utilities/Logger.h"

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

void TestPlane::BeginPlay()
{
    MeshDesc meshDesc;
    meshDesc.m_numVerts = sizeof(vertices) / (sizeof(GLfloat) * 3);
    meshDesc.m_vertices.assign(vertices, vertices + meshDesc.m_numVerts * 3);
    meshDesc.m_uvCoords.assign(uvs, uvs + meshDesc.m_numVerts * 2);
    meshDesc.m_normals.resize(meshDesc.m_numVerts * 3, 0.0f);
    meshDesc.m_indices.assign(indices, indices + sizeof(indices) / sizeof(GLuint));
    
    m_planeMesh = ResourceManager::Get().CreateResource<MeshResource>("plane");
    if (m_planeMesh.IsValid())
    {
        m_planeMesh->AddSubMesh(meshDesc);
    }
    
    if (MeshComponent* meshComponent = SceneManager::Get().GetComponent<MeshComponent>(m_planeEntity))
    {
        meshComponent->m_meshHandle = m_planeMesh;
    }
}

void TestPlane::Update(float deltaTime)
{
    m_timer += deltaTime;
    
	if (TransformComponent* transformComponent = SceneManager::Get().GetComponent<TransformComponent>(m_planeEntity))
	{
	    const Transform& transform = transformComponent->GetTransform();
	    const glm::vec3& rotation = transform.m_rotation;
	    transformComponent->SetRotation({ 10 * m_timer, rotation.y, rotation.z });
	    
	    const glm::vec3& position = transform.m_position;
	    transformComponent->SetPosition({ position.x, position.y, sin(m_timer) / 2.0f });
	    
	    transformComponent->SetScale(glm::vec3(1.0f) * (sin(m_timer) / 2.0f + 1.0f));
	}
}

void TestPlane::EndPlay()
{
    m_planeMesh.Reset();
}

namespace
{
    void DeserializeEntity(
        const nlohmann::json& data,
        const char* key,
        const EntityIDMapping& mapping,
        entt::entity& outEntity
        )
    {
        if (!data.contains(key) || data[key].is_null())
        {
            outEntity = entt::null;
            return;
        }

        if (!data[key].is_number_unsigned())
        {
            LOG_ERROR("PlayerMovement {} must be an unsigned integer or null", key);
            outEntity = entt::null;
            return;
        }

        uint32_t rawEntityID = data[key];
        outEntity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (outEntity == entt::null)
        {
            LOG_ERROR("PlayerMovement {} references unknown entity id {}", key, rawEntityID);
        }
    }
}

DEFINE_SCRIPT_DESERIALIZE_FUNC(Deserialize, TestPlane)
{
    ::DeserializeEntity(data, "planeEntity", mapping, script.m_planeEntity);
}

DEFINE_SCRIPT_COPY_FUNC(Copy, TestPlane)
{
    to.m_planeEntity = mapping.Get(from.m_planeEntity);
}

REGISTER_SCRIPT_FACTORY(TestPlane, TestPlane::Deserialize, TestPlane::Copy)
