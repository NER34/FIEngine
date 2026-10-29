#pragma once

#include <functional>
#include <string>

#include "json.h"

#include "Registry/Registry.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "Utilities/Logger.h"

class IMaterialParamFactory;

class MaterialParamRegistry : public Registry<std::string, IMaterialParamFactory>
{
public:
    static MaterialParamRegistry& Get()
    {
        static MaterialParamRegistry instance;
        return instance;
    }
};

class IMaterialParamFactory
{
public:
    virtual ~IMaterialParamFactory() = default;
    virtual bool CreateFromAsset(MaterialResource& material, const std::string& name, const nlohmann::json& data) const = 0;
};

template <typename TMaterialParam>
class MaterialParamFactory : public IMaterialParamFactory
{
    using DeserializeFunc = std::function<bool(TMaterialParam&, const nlohmann::json&)>;
public:
    
    MaterialParamFactory(DeserializeFunc deserialize)
        : m_deserialize(deserialize)
    {}
    
    ~MaterialParamFactory() override = default;
    
    bool CreateFromAsset(MaterialResource& material, const std::string& name, const nlohmann::json& data) const final
    {
        TMaterialParam* parameter = material.PushParameter<TMaterialParam>(name);
        if (!parameter)
        {
            LOG_ERROR("Failed to create material parameter {}", name);
            return false;
        }
        parameter->SetName(name);
        return m_deserialize(*parameter, data);
    }
    
private:
    
    DeserializeFunc m_deserialize;
    static bool     s_registered;
    
};


#define DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(FuncName, TMaterialParam) \
    static bool FuncName(TMaterialParam& parameter, const nlohmann::json& data)

#define DEFINE_MATERIAL_PARAM_DESERIALIZE_FUNC(FuncName, TMaterialParam) \
    bool TMaterialParam::FuncName(TMaterialParam& parameter, const nlohmann::json& data)


#define MATERIAL_PARAM_FACTORY_REGISTER_VAR(TMaterialParam) template<> bool MaterialParamFactory<TMaterialParam>::s_registered
#define MATERIAL_PARAM_FACTORY_REGISTER_FUNC(TMaterialParam) MaterialParamRegistry::Get().FindObject<MaterialParamFactory<TMaterialParam>>

#define REGISTER_MATERIAL_PARAM_FACTORY(TMaterialParam, DeserializeFunc) \
    MATERIAL_PARAM_FACTORY_REGISTER_VAR(TMaterialParam) = MATERIAL_PARAM_FACTORY_REGISTER_FUNC(TMaterialParam) \
    ( \
        #TMaterialParam, DeserializeFunc \
    );
