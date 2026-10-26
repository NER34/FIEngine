#include "GameObject/Components/TransformComponent.h"

#include "GameObject/ComponentAllocator.h"
#include "GameObject/ComponentFactory.h"
#include "Utilities/Logger.h"

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
            LOG_ERROR("Transform component {} must contain three values", key);
            return;
        }

        for (std::size_t index = 0; index < 3; ++index)
        {
            if (!value[index].is_number())
            {
                LOG_ERROR("Transform component {} has an invalid value", key);
                return;
            }
            outVector[index] = value[index];
        }
    }
}

void Deserialize(
    TransformComponent& component,
    const nlohmann::json& data,
    const std::unordered_map<uint32_t, entt::entity>& entityIDMap
    )
{
    DeserializeVector3(data, "position", component.m_transform.m_position);
    DeserializeVector3(data, "rotation", component.m_transform.m_rotation);
    DeserializeVector3(data, "scale", component.m_transform.m_scale);
}

REGISTER_COMPONENT_FACTORY(TransformComponent, Deserialize)
