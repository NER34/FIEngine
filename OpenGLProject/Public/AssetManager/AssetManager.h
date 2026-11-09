#pragma once
#include <json.h>

#include "AssetFactory.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Utilities/ParsingHelpers.h"

class AssetManager
{
public:
    
    static AssetManager& Get()
    {
        static AssetManager instance;
        return instance;
    }
    
    template <typename TResource>
    bool Load(const std::string& path, ResourceHandle<TResource>& outHandle)
    {
        if (ResourceManager::Get().GetResource<TResource>(path, outHandle))
        {
            return true;
        }

        nlohmann::json asset;
        if (!LoadAsset(path, asset))
        {
            return false;
        }

        if (!HasStringField(asset, "type"))
        {
            LOG_ERROR("Invalid asset file {}: type is missing", path);
            return false;
        }

        const std::string type = asset["type"];
        IAssetFactory* assetFactory = AssetRegistry::Get().GetObject(type);
        if (!assetFactory)
        {
            LOG_ERROR("Invalid asset file {}: unknown type {}", path, type);
            return false;
        }
        if (assetFactory->GetResourceType() != typeid(TResource))
        {
            LOG_ERROR("Invalid asset file {}: type {} doesn't match requested resource type", path, type);
            return false;
        }

        if (!assetFactory->CreateFromAsset(path, asset))
        {
            LOG_ERROR("Failed to create resource from asset file {}", path);
            ResourceManager::Get().FreeResource<TResource>(path);
            return false;
        }

        return ResourceManager::Get().GetResource<TResource>(path, outHandle);
    }

    template <typename TResource>
    bool IsLoaded(const std::string& path) const
    {
        ResourceHandle<TResource> handle;
        return ResourceManager::Get().GetResource<TResource>(path, handle);
    }

private:
    
    bool LoadAsset(const std::string& path, nlohmann::json& asset);
    
};
