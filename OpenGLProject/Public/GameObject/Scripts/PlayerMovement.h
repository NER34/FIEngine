#pragma once

#include <utility>
#include <vector>

#include "GameObject/ScriptBase.h"
#include "InputSystem/InputContext.h"

class PlayerMovement : public ScriptBase
{
public:

    void BeginPlay() override;
    void Update(float deltaTime) override;
    void EndPlay() override;
    void Deserialize(const nlohmann::json& data, const std::unordered_map<uint32_t, entt::entity>& entityIDMap) override;
    void Copy(const ScriptBase* other, const std::unordered_map<entt::entity, entt::entity>&) override;

private:

    float m_cameraMovSpeed = 10.0f;
    float m_cameraRotSpeed = 100.0f;
    bool m_followMouse = false;
    bool m_attachToLight = false;

    struct CameraMovDir
    {
        int m_forwardMult = 0;
        int m_rightMult = 0;
        int m_upMult = 0;
    } m_cameraMovDir;

    struct CameraRot
    {
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;

        void zero() { m_pitch = 0.0f; m_yaw = 0.0f; }
    } m_cameraRot;

    entt::entity m_cameraEntity = entt::null;
    entt::entity m_lightEntity = entt::null;

    std::vector<std::pair<FIE::InputIDs, FIE::DispatcherHandle>> m_inputHandles;

};
