#pragma once

#include <typeindex>

class ShaderProgram;

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
    
    IMaterialParameter() = delete;
    virtual ~IMaterialParameter() = default;
    virtual void ApplyParameter(ShaderProgram* shaderProgram) const = 0;
    
private:
    
    friend class MaterialParameters;
    
    template <typename TSelf>
    friend class MaterialParameterBase;
    
    IMaterialParameter(std::type_index type)
        : m_type(type)
    {} 
    
    std::type_index GetParameterType() const
    {
        return m_type;
    }
    
private:
    
    std::type_index m_type;
    
};

template <typename TSelf>
class MaterialParameterBase : public IMaterialParameter
{    
public:
    
    MaterialParameterBase()
        : IMaterialParameter(std::type_index(typeid(TSelf)))
    {}

    static std::type_index GetStaticParameterType()
    {
        return std::type_index(typeid(TSelf));
    }
    
};
