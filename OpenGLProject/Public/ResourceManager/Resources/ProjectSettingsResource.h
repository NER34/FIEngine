#pragma once

#include "AssetManager/AssetFactory.h"
#include "ResourceManager/ResourceBase.h"

class ProjectSettingsResource : public ResourceBase
{
public:
    
    std::string m_windowName = "Default Window";
    int m_windowWidth = 600;
    int m_windowHeight = 400;
    
    std::string m_startScene;

    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, ProjectSettingsResource);
    
};
