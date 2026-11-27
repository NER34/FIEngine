#pragma once

#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/rotate_vector.hpp"

static  std::filesystem::path GetWorkingDirectoryPath()
{
    return std::filesystem::current_path();
}

static  std::filesystem::path GetResourcesPath()
{
#ifdef RESOURCES_FOLDER_PATH
    return RESOURCES_FOLDER_PATH;
#else
    return GetWorkingDirectoryPath() / "Resources";
#endif
}

static std::filesystem::path GetSavedDirectoryPath()
{
#ifdef SAVED_FOLDER_PATH
    return SAVED_FOLDER_PATH;
#else
    return GetWorkingDirectoryPath() / "Saved";
#endif
}

inline glm::vec3 RotateVector(glm::vec3 vector, glm::vec3 eulerRot)
{
    vector = glm::rotateX(vector, glm::radians(eulerRot.x));
    vector = glm::rotateY(vector, glm::radians(eulerRot.y));
    vector = glm::rotateZ(vector, glm::radians(eulerRot.z));
    return vector;
}

struct Transform
{
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    
    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, m_position);
        modelMatrix *= glm::eulerAngleZYX(
            glm::radians(m_rotation.z), 
            glm::radians(m_rotation.y), 
            glm::radians(m_rotation.x)
            );
        modelMatrix = glm::scale(modelMatrix, m_scale);
        return modelMatrix;
    }
    
    glm::vec3 GetForwardVector() const
    {
        return RotateVector(glm::vec3(1.0f, 0.0f, 0.0f), m_rotation);
    }
    
    glm::vec3 GetRightVector() const
    {
        return RotateVector(glm::vec3(0.0f, 1.0f, 0.0f), m_rotation);
    }
    
    glm::vec3 GetUpVector() const
    {
        return RotateVector(glm::vec3(0.0f, 0.0f, 1.0f), m_rotation);
    }
};

inline glm::vec3 ToOGLCoordSys(glm::vec3 coords)
{
    return glm::vec3(coords.y, coords.z, -coords.x);
}

constexpr glm::mat4 toLeftHandTransformMat = {
    {  0.0f, 0.0f, -1.0f, 0.0f },
    {  1.0f, 0.0f,  0.0f, 0.0f },
    {  0.0f, 1.0f,  0.0f, 0.0f },
    {  0.0f, 0.0f,  0.0f, 1.0f },
};

inline glm::mat4 ToOGLCoordSys(const glm::mat4& mat)
{
    return toLeftHandTransformMat * mat;
}

