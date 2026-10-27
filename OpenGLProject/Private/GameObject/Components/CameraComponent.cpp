#include "GameObject/Components/CameraComponent.h"

#include "GameObject/ComponentAllocator.h"
#include "GameObject/ComponentFactory.h"
#include "Utilities/Logger.h"

glm::mat4 CameraComponent::GetViewMatrix(const Transform& transform) const
{
    return glm::lookAt(
        ToOGLCoordSys(transform.m_position),
        ToOGLCoordSys(transform.m_position + transform.GetForwardVector()),
        ToOGLCoordSys(m_up)
        );
}

glm::mat4 CameraComponent::GetProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(m_fov), aspectRatio, m_nearClipPlane, m_farClipPlane);
}

namespace
{
    void DeserializeVector3(const nlohmann::json& data, const char* key, glm::vec3& outVector)
    {
        if (!data.contains(key))
        {
            return;
        }

        const nlohmann::json& value = data[key];
        if (!value.is_array() || value.size() != 3)
        {
            LOG_ERROR("Camera component {} must contain three values", key);
            return;
        }

        for (std::size_t index = 0; index < 3; ++index)
        {
            if (!value[index].is_number())
            {
                LOG_ERROR("Camera component {} has an invalid value", key);
                return;
            }
            outVector[index] = value[index];
        }
    }

    void DeserializeFloat(const nlohmann::json& data, const char* key, float& outValue)
    {
        if (!data.contains(key))
        {
            return;
        }

        if (!data[key].is_number())
        {
            LOG_ERROR("Camera component {} has an invalid value", key);
            return;
        }
        outValue = data[key];
    }

    IMPLEMENT_COMPONENT_DESERIALIZE(Deserialize, CameraComponent)
    {
        DeserializeVector3(data, "up", component.m_up);
        DeserializeFloat(data, "fov", component.m_fov);
        DeserializeFloat(data, "nearClipPlane", component.m_nearClipPlane);
        DeserializeFloat(data, "farClipPlane", component.m_farClipPlane);
    }
}

REGISTER_COMPONENT_FACTORY(CameraComponent, ::Deserialize, [](auto&...){})
