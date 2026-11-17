#include "Scripts/SpawnForest.h"

#include "AssetManager/AssetManager.h"
#include "Components/TransformComponent.h"
#include "Scene/SceneManager.h"
#include "Utilities/ParsingHelpers.h"

#include <cmath>
#include <cstdlib>

#include "glm/gtc/constants.hpp"


void SpawnForest::Update(float deltaTime)
{
    if (m_numTreesSpawned >= m_numTrees || !m_scene.IsValid())
    {
        return;
    }

    m_timer += deltaTime;
    if (m_timer < m_timeout) { return; }
    m_timer = 0.0f;

    // Count the attempt even on failure so a broken scene doesn't retry forever
    m_numTreesSpawned++;

    SceneManager& sceneManager = SceneManager::Get();
    const entt::entity entity = sceneManager.SpawnScene(m_scene);
    if (entity == entt::null)
    {
        LOG_ERROR("SpawnForest: failed to spawn scene");
        return;
    }

    TransformComponent* transform = sceneManager.GetComponent<TransformComponent>(entity);
    if (!transform)
    {
        LOG_ERROR("SpawnForest: spawned scene root has no TransformComponent");
        return;
    }

    // Uniform random point inside a disk of m_spawnRadius
    const float angle = (std::rand() / static_cast<float>(RAND_MAX)) * glm::two_pi<float>();
    const float radius = m_spawnRadius * std::sqrt(std::rand() / static_cast<float>(RAND_MAX));
    transform->SetPosition(glm::vec3(radius * std::cos(angle), radius * std::sin(angle), 0.0f));
}

DEFINE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SpawnForest)
{
    if (!HasStringField(data, "scene"))
    {
        LOG_ERROR("SpawnForest script must contain a scene path");
    }
    else
    {
        const std::string& scenePath = data["scene"];
        if (!AssetManager::Get().Load<SceneResource>(scenePath, script.m_scene))
        {
            LOG_ERROR("SpawnForest script failed to load scene {}", scenePath);
        }
    }
    DeserializeInteger(data, "count", script.m_numTrees);
    DeserializeFloat(data, "timeout", script.m_timeout);
    DeserializeFloat(data, "radius", script.m_spawnRadius);
}

DEFINE_SCRIPT_COPY_FUNC(Copy, SpawnForest)
{
    to.m_scene = from.m_scene;
    to.m_numTrees = from.m_numTrees;
    to.m_timeout = from.m_timeout;
    to.m_spawnRadius = from.m_spawnRadius;
}

REGISTER_SCRIPT_FACTORY(SpawnForest, SpawnForest::Deserialize, SpawnForest::Copy)
