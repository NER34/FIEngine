#include "Components/LightComponent.h"

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
            LOG_ERROR("Light component {} must contain three values", key);
            return;
        }

        for (std::size_t index = 0; index < 3; ++index)
        {
            if (!value[index].is_number())
            {
                LOG_ERROR("Light component {} has an invalid value", key);
                return;
            }
            outVector[index] = value[index];
        }
    }
}

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, LightComponent)
{
    auto& component = handle.m_registry.get<LightComponent>(handle.m_entity);
    
    ::DeserializeVector3(data, "lightColor", component.m_lightColor);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, LightComponent)
{
    auto& to = handle_to.m_registry.get<LightComponent>(handle_to.m_entity);
    const auto& from = handle_from.m_registry.get<LightComponent>(handle_from.m_entity);
    
    to.m_lightColor = from.m_lightColor;
}

REGISTER_COMPONENT_FACTORY(LightComponent, LightComponent::Deserialize, LightComponent::Copy)
