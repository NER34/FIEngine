#pragma once

#include "glm/glm.hpp"
#include "Utilities/Utilities.h"

struct CameraDesc
{
    glm::vec3 m_up = glm::vec3(0.0f, 0.0f, 1.0f);
    float m_fov = 90.0f;
    float m_aspectRatio = 640.0f / 480.0f;
    float m_nearClipPlane = 0.1f;
    float m_farClipPlane = 100.0f;
};

class Camera
{
public:
    
    Camera() = default;
    explicit Camera(const CameraDesc& desc)
        : m_cameraDesc(desc)
    {}
    
    Camera(const Camera& other) = default;
    Camera& operator=(const Camera& other) = default;
    
    Camera(Camera&& other) noexcept = default;
    Camera& operator=(Camera&& other) noexcept = default;
    
    const CameraDesc& getCameraDesc() const;
    
    const glm::vec3& getPosition() const;
    const glm::vec3& getRotation() const;
    const Transform& getTransform() const;
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    
    void setFOV(float fov);
    void setAspectRatio(float aspectRatio);
    void setClipPlanes(float nearClipPlane, float farClipPlane);
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setTransform(const Transform& transform);
    
private:
    
    CameraDesc m_cameraDesc;
    Transform m_transform;
    
};
