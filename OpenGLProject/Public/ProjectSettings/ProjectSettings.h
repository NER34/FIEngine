#pragma once

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ProjectSettingsResource.h"
#include "SystemBase/SystemLocator.h"

class ProjectSettings : public ISystemBase
{
public:
    
    ProjectSettings() = default;
    ~ProjectSettings() override = default;
    
    void Initialize() override;
    void Deinitialize() override;
    
    const ProjectSettingsResource* operator->() const
    {
        return m_settingsHandle.operator->();
    }
    
    ProjectSettingsResource* operator->()
    {
        return m_settingsHandle.operator->();
    }
    
private:
    
    ResourceHandle<ProjectSettingsResource> m_settingsHandle;
    
};
