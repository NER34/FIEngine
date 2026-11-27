#include "AssetManager.h"

#include <fstream>

#include "AssetFactory.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

bool AssetManager::LoadAsset(const std::string& path, nlohmann::json& asset)
{
    const std::string fullPath = (GetResourcesPath() / path).string();
    std::ifstream assetFile(fullPath);
    if (!assetFile.is_open())
    {
        LOG_ERROR("Failed to open asset file: {}", path);
        return false;
    }

    try
    {
        assetFile >> asset;
        return true;
    }
    catch (const nlohmann::json::exception& exception)
    {
        LOG_ERROR("Failed to parse asset file {}: {}", path, exception.what());
        return false;
    }
}
