#pragma once

#include <cstdint>
#include <json.h>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <utility>

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
    virtual void ApplyParameter() const = 0;
    
    //virtual void Deserialize(const std::string& name, unsigned int shaderProgramID, nlohmann::json& data) = 0;
    
private:
    
    friend class MaterialParametersPool;
    
    template <typename TSelf>
    friend class MaterialParameterBase;
    
    IMaterialParameter(std::type_index type)
        : m_type(type)
    {} 
    
    std::type_index GetParameterType() const
    {
        return m_type;
    }
    
    virtual uint32_t GetParameterSize() const = 0;
    
    virtual void CopyParameter(void* buffer = nullptr) = 0;
    virtual void MoveParameter(void* buffer = nullptr) noexcept = 0;
    
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
    
    uint32_t GetParameterSize() const final
    {
        return sizeof(TSelf);
    }
    
private:
    
    void CopyParameter(void* buffer) final
    {
        if (buffer)
        {
            new (buffer) TSelf(*static_cast<TSelf*>(this));
        }
    }
    
    void MoveParameter(void* buffer) noexcept final
    {
        static_assert(std::is_nothrow_move_constructible_v<TSelf>);
        
        if (buffer)
        {
            new (buffer) TSelf(std::move(*static_cast<TSelf*>(this)));
        }
    }
    
};
