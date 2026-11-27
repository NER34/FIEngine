#pragma once

#include "glm/glm.hpp"

#include "Utilities/Utilities.h"
#include "ComponentFactory/ComponentFactory.h"

class CameraComponent
{
public:
    
    glm::vec3 m_up = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_fov = 90.0f;
    float m_nearClipPlane = 0.1f;
    float m_farClipPlane = 100.0f;
    
    glm::mat4 GetViewMatrix(const Transform& transform) const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, CameraComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, CameraComponent);
    
};
