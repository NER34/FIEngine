#pragma once

#include <json.h>
#include <typeindex>

#include "ResourceManager/ResourceHandle.h"
#include "Utilities/Logger.h"

class ShaderProgramResource;

enum class MaterialParamType : unsigned char
{
    Float = 0,
    Vec3,
    Mat4,
    Texture,
    
    Invalid = 255
};

class IMaterialParameter
{
public:
    
    IMaterialParameter() = default;
    virtual ~IMaterialParameter() = default;
    
    virtual void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const = 0;
    
    virtual bool Deserialize(const nlohmann::json& data)
    {
        if (!data.contains("name") || !data["name"].is_string())
        {
            LOG_ERROR("Material parameter is missing its name");
            return false;
        }
        SetName(data["name"]);
        return true;
    }
    
    void SetName(const std::string& name)
    {
        m_name = name;
    }
    
    const std::string& GetName() const
    {
        return m_name;
    }
    
private:
    
    friend class MaterialParameters;
    
    virtual std::type_index GetParameterType() const = 0;
    
protected:
    
    std::string m_name;
    
};

template <typename TSelf>
class MaterialParameterBase : public IMaterialParameter
{    
public:
    
    MaterialParameterBase()
        : m_type(std::type_index(typeid(TSelf)))
    {}

    static std::type_index GetStaticParameterType()
    {
        return std::type_index(typeid(TSelf));
    }
    
private:
    
    std::type_index GetParameterType() const final
    {
        return m_type;
    }
    
    std::type_index m_type;
    
};
