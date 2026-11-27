#pragma once

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ProjectSettingsResource.h"
#include "SystemBase/SystemLocator.h"

class ProjectSettings : public ISystemBase
{
public:
    
    static ProjectSettings* Get();
    
    ProjectSettings() = default;
    ~ProjectSettings() override = default;
    
    void Initialize() override;
    void Deinitialize() override;

    bool IsValid() const { return m_settingsHandle.IsValid(); }

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
