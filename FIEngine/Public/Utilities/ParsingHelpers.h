#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

#include <json.h>

#include "entt.h"
#include "glm/glm.hpp"

#include "ComponentFactory/ComponentFactoryStructs.h"
#include "Utilities/Logger.h"

inline bool HasStringField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_string();
}

inline bool HasNumberField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_number();
}

inline bool HasIntegerField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_number_integer();
}

inline bool HasUnsignedField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_number_unsigned();
}

inline bool HasArrayField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_array();
}

inline bool HasObjectField(const nlohmann::json& data, const char* key)
{
    return data.contains(key) && data[key].is_object();
}

// True when the field is absent or is an array
inline bool IsOptionalArrayField(const nlohmann::json& data, const char* key)
{
    return !data.contains(key) || data[key].is_array();
}

inline void DeserializeFloat(const nlohmann::json& data, const char* key, float& outValue)
{
    if (!data.contains(key))
    {
        return;
    }

    if (!data[key].is_number())
    {
        LOG_ERROR("Field {} has an invalid value", key);
        return;
    }
    outValue = data[key];
}

template <typename T>
void DeserializeInteger(const nlohmann::json& data, const char* key, T& outValue)
{
    static_assert(std::is_integral_v<T>, "DeserializeInteger requires an integral type");

    if (!data.contains(key))
    {
        return;
    }

    if (!data[key].is_number_integer())
    {
        LOG_ERROR("Field {} has an invalid value", key);
        return;
    }
    outValue = data[key].get<T>();
}

inline void DeserializeString(const nlohmann::json& data, const char* key, std::string& outValue)
{
    if (!data.contains(key))
    {
        return;
    }

    if (!data[key].is_string())
    {
        LOG_ERROR("Field {} has an invalid value", key);
        return;
    }
    outValue = data[key].get<std::string>();
}

inline void DeserializeBool(const nlohmann::json& data, const char* key, bool& outValue)
{
    if (!data.contains(key))
    {
        return;
    }

    if (!data[key].is_boolean())
    {
        LOG_ERROR("Field {} has an invalid value", key);
        return;
    }
    outValue = data[key].get<bool>();
}

inline void DeserializeVector3(const nlohmann::json& data, const char* key, glm::vec3& outVector)
{
    if (!data.contains(key))
    {
        return;
    }

    const nlohmann::json& value = data[key];
    if (!value.is_array() || value.size() != 3)
    {
        LOG_ERROR("Field {} must contain three values", key);
        return;
    }

    for (std::size_t index = 0; index < 3; ++index)
    {
        if (!value[index].is_number())
        {
            LOG_ERROR("Field {} has an invalid value", key);
            return;
        }
        outVector[index] = value[index];
    }
}

inline void DeserializeEntity(
    const nlohmann::json& data,
    const char* key,
    const EntityIDMapping& mapping,
    entt::entity& outEntity
    )
{
    if (!data.contains(key) || data[key].is_null())
    {
        outEntity = entt::null;
        return;
    }

    if (!data[key].is_number_unsigned())
    {
        LOG_ERROR("Field {} must be an unsigned integer or null", key);
        outEntity = entt::null;
        return;
    }

    uint32_t rawEntityID = data[key];
    outEntity = mapping.Get(static_cast<entt::entity>(rawEntityID));
    if (outEntity == entt::null)
    {
        LOG_ERROR("Field {} references unknown entity id {}", key, rawEntityID);
    }
}
