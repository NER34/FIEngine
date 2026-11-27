#include "Components/LightComponent.h"

#include "Utilities/ParsingHelpers.h"

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
