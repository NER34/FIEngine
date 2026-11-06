#pragma once

#include <utility>
#include <vector>

#include "Scripting/ScriptBase.h"
#include "Scripting/ScriptFactory.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MeshResource.h"

class TestPlane : public ScriptBase
{
public:

    void BeginPlay() override;
    void Update(float deltaTime) override;
    void EndPlay() override;

    DECLARE_SCRIPT_DESERIALIZE_FUNC(Deserialize, TestPlane);
    DECLARE_SCRIPT_COPY_FUNC(Copy, TestPlane);

private:

    float m_timer = 0.0f;

    entt::entity m_planeEntity = entt::null;
    ResourceHandle<MeshResource> m_planeMesh;

};
