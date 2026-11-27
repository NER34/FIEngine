#pragma once

#include "Scripting/ScriptBase.h"
#include "Scripting/ScriptFactory.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/SceneResource.h"

class SpawnForest : public ScriptBase
{
public:

    void Update(float deltaTime) override;

    DECLARE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SpawnForest);
    DECLARE_SCRIPT_COPY_FUNC(Copy, SpawnForest);

private:

    int m_numTreesSpawned = 0;
    int m_numTrees = 0;
    float m_timer = 0.0f;
    float m_timeout = 0.0f;
    float m_spawnRadius = 1000.0f;
    ResourceHandle<SceneResource> m_scene;

};
