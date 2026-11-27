#pragma once

#include "Scripting/ScriptBase.h"
#include "Scripting/ScriptFactory.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/SceneResource.h"

class SpawnScene : public ScriptBase
{
public:

    void BeginPlay() override;

    DECLARE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SpawnScene);
    DECLARE_SCRIPT_COPY_FUNC(Copy, SpawnScene);

private:

    ResourceHandle<SceneResource> m_scene;

};
