#include "ResourceManager/Resources/ProjectSettingsResource.h"

#include "Utilities/ParsingHelpers.h"

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, ProjectSettingsResource)
{
    DeserializeString(data, "windowName", resource.m_windowName);
    DeserializeInteger(data, "windowWidth", resource.m_windowWidth);
    DeserializeInteger(data, "windowHeight", resource.m_windowHeight);
    
    DeserializeString(data, "startScene", resource.m_startScene);
    
    return true;
}

REGISTER_ASSET_FACTORY(ProjectSettingsResource, &ProjectSettingsResource::Deserialize)
