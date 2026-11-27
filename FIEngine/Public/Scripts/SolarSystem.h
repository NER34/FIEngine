#pragma once

#include "Scripting/ScriptBase.h"
#include "Scripting/ScriptFactory.h"

class SolarSystem : public ScriptBase
{
public:

    void Update(float deltaTime) override;

    DECLARE_SCRIPT_DESERIALIZE_FUNC(Deserialize, SolarSystem);
    DECLARE_SCRIPT_COPY_FUNC(Copy, SolarSystem);

private:

    entt::entity m_moon = entt::null;
    entt::entity m_earth = entt::null;
    entt::entity m_earth_proxy = entt::null;
    entt::entity m_sun = entt::null;
    entt::entity m_sun_proxy = entt::null;

};
