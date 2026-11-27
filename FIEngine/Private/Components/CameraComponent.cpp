#include "Components/CameraComponent.h"

#include "Utilities/ParsingHelpers.h"

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

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, CameraComponent)
{
    auto& component = handle.m_registry.get<CameraComponent>(handle.m_entity);
    
    ::DeserializeVector3(data, "up", component.m_up);
    ::DeserializeFloat(data, "fov", component.m_fov);
    ::DeserializeFloat(data, "nearClipPlane", component.m_nearClipPlane);
    ::DeserializeFloat(data, "farClipPlane", component.m_farClipPlane);
}

DEFINE_COMPONENT_COPY_FUNC(Copy, CameraComponent)
{
    auto& to = handle_to.m_registry.get<CameraComponent>(handle_to.m_entity);
    const auto& from = handle_from.m_registry.get<CameraComponent>(handle_from.m_entity);
    
    to.m_up = from.m_up;
    to.m_fov = from.m_fov;
    to.m_nearClipPlane = from.m_nearClipPlane;
    to.m_farClipPlane = from.m_farClipPlane;
}

REGISTER_COMPONENT_FACTORY(CameraComponent, CameraComponent::Deserialize, CameraComponent::Copy)
