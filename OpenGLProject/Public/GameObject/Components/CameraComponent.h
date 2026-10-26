#pragma once

#include "glm/vec3.hpp"

class CameraComponent
{
public:
    glm::vec3 m_up = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_fov = 90.0f;
    float m_nearClipPlane = 0.1f;
    float m_farClipPlane = 100.0f;
};
