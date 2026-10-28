#include "Scripts/PlayerMovement.h"

#include <algorithm>

#include "Scripting/ScriptFactory.h"
#include "Components/TransformComponent.h"
#include "InputSystem/InputManager.h"
#include "Scene/SceneManager.h"
#include "Utilities/Logger.h"

void PlayerMovement::BeginPlay()
{
    FIE::IInputManager& inputManager = FIE::IInputManager::Get();
    FIE::InputContext* inputContext = inputManager.getInputContext();

    auto assignHandle = [&](FIE::InputIDs inputID, FIE::InputContext::InputWeekHandle&& handle)
    {
        m_inputHandles.emplace_back(inputID, inputContext->assignHandle(inputID, std::move(handle)));
    };

#define CHANGE_CAMERA_POS(dir, multi) \
	[this](FIE::InputIDs inputID, float inputValue) \
	{ \
		m_cameraMovDir.dir += inputValue > 0.5f ? multi : -(multi); \
	}

    assignHandle(FIE::KEY_W, CHANGE_CAMERA_POS(m_forwardMult, 1));
    assignHandle(FIE::KEY_S, CHANGE_CAMERA_POS(m_forwardMult, -1));
    assignHandle(FIE::KEY_D, CHANGE_CAMERA_POS(m_rightMult, 1));
    assignHandle(FIE::KEY_A, CHANGE_CAMERA_POS(m_rightMult, -1));
    assignHandle(FIE::KEY_E, CHANGE_CAMERA_POS(m_upMult, 1));
    assignHandle(FIE::KEY_Q, CHANGE_CAMERA_POS(m_upMult, -1));

#undef CHANGE_CAMERA_POS

#define CHANGE_CAMERA_ROT(axis) \
	[this](FIE::InputIDs inputID, float inputValue) \
	{ \
		m_cameraRot.axis = inputValue; \
	}

    assignHandle(FIE::MOUSE_MOVE_X, CHANGE_CAMERA_ROT(m_yaw));
    assignHandle(FIE::MOUSE_MOVE_Y, CHANGE_CAMERA_ROT(m_pitch));

#undef CHANGE_CAMERA_ROT

	assignHandle(FIE::MOUSE_BUTTON_LEFT,
		[this](FIE::InputIDs inputID, float inputValue)
		{
			m_followMouse = inputValue > 0.5f;
		}
	);

	assignHandle(FIE::KEY_1,
		[this](FIE::InputIDs inputID, float inputValue)
		{
			if (inputValue > 0.5f)
				m_attachToLight = !m_attachToLight;
		}
	);
}

void PlayerMovement::Update(float deltaTime)
{
	if (!m_followMouse)
	{
		return;
	}

	auto transformView = SceneManager::Get().View<TransformComponent>();
	if (!transformView.contains(m_cameraEntity))
	{
		return;
	}

	TransformComponent& cameraTransformComp = transformView.get<TransformComponent>(m_cameraEntity);
	Transform& cameraTransform = cameraTransformComp.m_transform;

	auto cameraForward = cameraTransform.GetForwardVector();
	auto cameraRight = cameraTransform.GetRightVector();

	auto& cameraPos = cameraTransform.m_position;
	cameraPos += cameraForward * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_forwardMult;
	cameraPos += cameraRight * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_rightMult;
	cameraPos += glm::vec3(0.0f, 0.0f, 1.0f) * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_upMult;

	float deltaPitch = m_cameraRot.m_pitch * deltaTime * m_cameraRotSpeed;
	float deltaYaw = m_cameraRot.m_yaw * deltaTime * m_cameraRotSpeed;
	m_cameraRot.zero();

	auto& cameraRot = cameraTransform.m_rotation;
	cameraRot += glm::vec3{ 0.0f, deltaPitch, deltaYaw };
	cameraRot.y = std::clamp(cameraRot.y, -89.0f, 89.0f);
	cameraTransformComp.m_bModified = true;

	if (m_attachToLight && transformView.contains(m_lightEntity))
	{
		TransformComponent& lightTransformComp = transformView.get<TransformComponent>(m_lightEntity);
		lightTransformComp.m_transform.m_position = cameraPos;
		lightTransformComp.m_bModified = true;
	}
}

void PlayerMovement::EndPlay()
{
    FIE::InputContext* inputContext = FIE::IInputManager::Get().getInputContext();
    for (const auto& [inputID, handle] : m_inputHandles)
    {
        inputContext->removeHandle(inputID, handle);
    }
    m_inputHandles.clear();
}

namespace
{
    void DeserializeEntity(
        const nlohmann::json& data,
        const char* key,
        const EntityIDMapping& mapping,
        entt::entity& outEntity
        )
    {
        if (!data.contains(key) || data[key].is_null())
        {
            outEntity = entt::null;
            return;
        }

        if (!data[key].is_number_unsigned())
        {
            LOG_ERROR("PlayerMovement {} must be an unsigned integer or null", key);
            outEntity = entt::null;
            return;
        }

        uint32_t rawEntityID = data[key];
        outEntity = mapping.Get(static_cast<entt::entity>(rawEntityID));
        if (outEntity == entt::null)
        {
            LOG_ERROR("PlayerMovement {} references unknown entity id {}", key, rawEntityID);
        }
    }
}

DEFINE_SCRIPT_DESERIALIZE_FUNC(Deserialize, PlayerMovement)
{
    ::DeserializeEntity(data, "cameraEntity", mapping, script.m_cameraEntity);
    ::DeserializeEntity(data, "lightEntity", mapping, script.m_lightEntity);

    if (data.contains("movementSpeed"))
    {
        if (data["movementSpeed"].is_number())
            script.m_cameraMovSpeed = data["movementSpeed"];
        else
            LOG_ERROR("PlayerMovement movementSpeed has an invalid value");
    }

    if (data.contains("rotationSpeed"))
    {
        if (data["rotationSpeed"].is_number())
            script.m_cameraRotSpeed = data["rotationSpeed"];
        else
            LOG_ERROR("PlayerMovement rotationSpeed has an invalid value");
    }
}

DEFINE_SCRIPT_COPY_FUNC(Copy, PlayerMovement)
{
    to.m_cameraMovSpeed = from.m_cameraMovSpeed;
    to.m_cameraRotSpeed = from.m_cameraRotSpeed;

    to.m_cameraEntity = mapping.Get(from.m_cameraEntity);
    to.m_lightEntity = mapping.Get(from.m_lightEntity);
}

REGISTER_SCRIPT_FACTORY(PlayerMovement, PlayerMovement::Deserialize, PlayerMovement::Copy)
