#include "Rendering/Camera.h"

#include "Utilities/Utilities.h"

const CameraDesc& Camera::getCameraDesc() const
{
    return m_cameraDesc;
}

const glm::vec3& Camera::getPosition() const
{
    return m_transform.m_position;
}

const glm::vec3& Camera::getRotation() const
{
    return m_transform.m_rotation;
}

const Transform& Camera::getTransform() const
{
    return m_transform;
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(
        toOGLCoordSys(m_transform.m_position),
        toOGLCoordSys(m_transform.m_position + m_transform.getForwardVector()),
        toOGLCoordSys(m_cameraDesc.m_up)
        );
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(
        glm::radians(m_cameraDesc.m_fov),
        m_cameraDesc.m_aspectRatio,
        m_cameraDesc.m_nearClipPlane,
        m_cameraDesc.m_farClipPlane
        );
}

void Camera::setFOV(float fov)
{
    m_cameraDesc.m_fov = fov;
}

void Camera::setAspectRatio(float aspectRatio)
{   
    m_cameraDesc.m_aspectRatio = aspectRatio;
}

void Camera::setClipPlanes(float nearClipPlane, float farClipPlane)
{
    m_cameraDesc.m_nearClipPlane = nearClipPlane;
    m_cameraDesc.m_farClipPlane = farClipPlane;
}

void Camera::setPosition(const glm::vec3& position)
{
    m_transform.m_position = position;
}

void Camera::setRotation(const glm::vec3& rotation)
{
    m_transform.m_rotation = rotation;
}

void Camera::setTransform(const Transform& transform)
{
    m_transform = transform;
}

