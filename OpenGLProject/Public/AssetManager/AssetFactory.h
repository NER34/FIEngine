#pragma once

#include <functional>
#include <string>
#include <typeindex>

#include <json.h>

#include "Registry/Registry.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"

class IAssetFactory;

class AssetRegistry : public Registry<std::string, IAssetFactory>
{
public:
    static AssetRegistry& Get()
    {
        static AssetRegistry instance;
        return instance;
    }
};

class IAssetFactory
{
public:
    virtual ~IAssetFactory() = default;
    virtual bool CreateFromAsset(const std::string& name, const nlohmann::json& data) const = 0;
    virtual std::type_index GetResourceType() const = 0;
};

template <typename TResource>
class AssetFactory : public IAssetFactory
{
    using TDeserializeFunc = std::function<bool(TResource&, const nlohmann::json&)>;
public:
    
    AssetFactory(TDeserializeFunc deserialize)
        : m_deserialize(deserialize)
    {}
    
    bool CreateFromAsset(const std::string& name, const nlohmann::json& data) const final
    {
        ResourceHandle<TResource> handle = ResourceManager::Get().CreateResource<TResource>(name);
        if (!handle.IsValid())
        {
            LOG_ERROR("Failed to create resource {}", name);
            return false;
        }
        return m_deserialize(*handle.operator->(), data);
    }
    
    std::type_index GetResourceType() const final
    {
        return typeid(TResource);
    }
    
private:
    
    TDeserializeFunc m_deserialize;
    static bool      s_registered;
    
};


#define DECLARE_ASSET_DESERIALIZE_FUNC(FuncName, TResource) \
    static bool FuncName(TResource& resource, const nlohmann::json& data)

#define DEFINE_ASSET_DESERIALIZE_FUNC(FuncName, TResource) \
    bool TResource::FuncName(TResource& resource, const nlohmann::json& data)


#define ASSET_FACTORY_REGISTER_VAR(TResource) template<> bool AssetFactory<TResource>::s_registered
#define ASSET_FACTORY_REGISTER_FUNC(TResource) AssetRegistry::Get().RegisterObject<AssetFactory<TResource>>

#define REGISTER_ASSET_FACTORY(TResource, DeserializeFunc) \
    ASSET_FACTORY_REGISTER_VAR(TResource) = ASSET_FACTORY_REGISTER_FUNC(TResource) \
    ( \
        #TResource, DeserializeFunc \
    );
