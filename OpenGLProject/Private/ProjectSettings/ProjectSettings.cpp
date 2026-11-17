#include "ProjectSettings/ProjectSettings.h"

#include "AssetManager/AssetManager.h"

namespace
{
    std::string g_settingsPath = "ProjectSettings.asset";
}

ProjectSettings* ProjectSettings::Get()
{
    if (ISystemBase* system = SystemLocator::Get().GetSystem<ProjectSettings>())
    {
        return static_cast<ProjectSettings*>(system);
    }
    return nullptr;
}

void ProjectSettings::Initialize()
{
     if (!AssetManager::Get().Load(g_settingsPath, m_settingsHandle))
     {
         LOG_ERROR("Failed to load project settings. Path: {}", g_settingsPath);
         m_settingsHandle = ResourceManager::Get().CreateResource<ProjectSettingsResource>(g_settingsPath);
     }
}

void ProjectSettings::Deinitialize()
{
    m_settingsHandle.Reset();
}
