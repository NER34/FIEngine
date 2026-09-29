#pragma once

#include <filesystem>

inline std::filesystem::path getWorkingDirectoryPath()
{
    return std::filesystem::current_path();
}

inline std::filesystem::path getResourcesPath()
{
    return getWorkingDirectoryPath() / "Resources";
}

template <bool inverseZ = true>
glm::vec3 toOGLCoordSys(glm::vec3 coords)
{
    constexpr int mask = 0 - (int)inverseZ;
    constexpr int multiplier = (-1 & mask) | (1 & ~mask);
    return glm::vec3(
        coords.y, 
        coords.z, 
        multiplier * coords.x
        );
}