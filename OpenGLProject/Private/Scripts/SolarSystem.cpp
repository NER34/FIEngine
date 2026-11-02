#include "Scripts/SolarSystem.h"

#include <cmath>

#include "Components/TransformComponent.h"
#include "Scene/SceneManager.h"

namespace
{
    // Degrees per second around the up (Z) axis
    constexpr float g_sunSpinSpeed      = 5.0f;
    constexpr float g_earthOrbitSpeed   = 10.0f;
    constexpr float g_earthSpinSpeed    = 60.0f;
    constexpr float g_moonOrbitSpeed    = 40.0f;

    void RotateAroundUp(TransformComponent& transform, float speed, float deltaTime)
    {
        glm::vec3 rotation = transform.GetTransform().m_rotation;
        rotation.z = std::fmod(rotation.z + speed * deltaTime, 360.0f);
        transform.SetRotation(rotation);
    }
}

void SolarSystem::Update(float deltaTime)
{
    TransformComponent* sunTransform = SceneManager::Get().GetComponent<TransformComponent>(m_sun);
    TransformComponent* earthTransform = SceneManager::Get().GetComponent<TransformComponent>(m_earth);
    TransformComponent* sunPTransform = SceneManager::Get().GetComponent<TransformComponent>(m_sun_proxy);
    TransformComponent* earthPTransform = SceneManager::Get().GetComponent<TransformComponent>(m_earth_proxy);

    // Spins
    RotateAroundUp(*sunTransform, g_sunSpinSpeed, deltaTime);
    RotateAroundUp(*earthTransform, g_earthSpinSpeed, deltaTime);

    // Orbits: rotating a proxy carries its children around the proxy's origin
    RotateAroundUp(*sunPTransform, g_earthOrbitSpeed, deltaTime);
    RotateAroundUp(*earthPTransform, g_moonOrbitSpeed, deltaTime);
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
            LOG_ERROR("SolarSystem {} must be an unsigned integer or null", key);
            outEntity = entt::null;
            return;
        }

        uint32_t rawEntityID = data[key];
        outEntity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (outEntity == entt::null)
        {
            LOG_ERROR("SolarSystem {} references unknown entity id {}", key, rawEntityID);
        }
    }
}

DEFINE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SolarSystem)
{
    ::DeserializeEntity(data, "sun", mapping, script.m_sun);
    ::DeserializeEntity(data, "sun_proxy", mapping, script.m_sun_proxy);
    ::DeserializeEntity(data, "earth", mapping, script.m_earth);
    ::DeserializeEntity(data, "earth_proxy", mapping, script.m_earth_proxy);
    ::DeserializeEntity(data, "moon", mapping, script.m_moon);
}

DEFINE_SCRIPT_COPY_FUNC(Copy, SolarSystem)
{
    to.m_sun = mapping.Get(from.m_sun);
    to.m_sun_proxy = mapping.Get(from.m_sun_proxy);
    to.m_earth = mapping.Get(from.m_earth);
    to.m_earth_proxy = mapping.Get(from.m_earth_proxy);
    to.m_moon = mapping.Get(from.m_moon);
}

REGISTER_SCRIPT_FACTORY(SolarSystem, SolarSystem::Deserialize, SolarSystem::Copy)
