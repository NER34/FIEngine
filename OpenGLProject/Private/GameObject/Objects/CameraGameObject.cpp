#include "GameObject/Objects/CameraGameObject.h"

#include "InputSystem/InputContext.h"
#include "InputSystem/InputManager.h"
#include "Rendering/Camera.h"
#include "Rendering/Renderer.h"

void CameraGameObject::BeginPlay()
{
    FIE::IInputManager& inputManager = FIE::IInputManager::Get();
    FIE::InputContext* inputContext = inputManager.getInputContext();
	
#define CHANGE_CAMERA_POS(dir, multi) \
	[this](FIE::InputIDs inputID, float inputValue) \
	{ \
		m_cameraMovDir.dir += inputValue > 0.5f ? multi : -(multi); \
	}
	
    inputContext->assignHandle(FIE::KEY_W, CHANGE_CAMERA_POS(m_forwardMult, 1));
    inputContext->assignHandle(FIE::KEY_S, CHANGE_CAMERA_POS(m_forwardMult, -1));
    inputContext->assignHandle(FIE::KEY_D, CHANGE_CAMERA_POS(m_rightMult, 1));
    inputContext->assignHandle(FIE::KEY_A, CHANGE_CAMERA_POS(m_rightMult, -1));
    inputContext->assignHandle(FIE::KEY_E, CHANGE_CAMERA_POS(m_upMult, 1));
    inputContext->assignHandle(FIE::KEY_Q, CHANGE_CAMERA_POS(m_upMult, -1));

#define CHANGE_CAMERA_ROT(axis) \
	[this](FIE::InputIDs inputID, float inputValue) \
	{ \
		m_cameraRot.axis = inputValue; \
	}
	
    inputContext->assignHandle(FIE::MOUSE_MOVE_X, CHANGE_CAMERA_ROT(m_yaw));
    inputContext->assignHandle(FIE::MOUSE_MOVE_Y, CHANGE_CAMERA_ROT(m_pitch));
	
	inputContext->assignHandle(FIE::MOUSE_BUTTON_LEFT, 	
		[this](FIE::InputIDs inputID, float inputValue)
		{
			m_followMouse = inputValue > 0.5f;
		}
	);
	
	inputContext->assignHandle(FIE::KEY_1, 
		[this](FIE::InputIDs inputID, float inputValue)
		{
			if (inputValue > 0.5f)
				m_attachToLight = !m_attachToLight;
		}
	);
}

void CameraGameObject::Update(float deltaTime)
{
	if (!m_followMouse)
	{
		return;
	}
	
	Camera& camera = Renderer::get().getCamera();
	
	auto cameraForward =  camera.getTransform().getForwardVector();
	auto cameraRight = camera.getTransform().getRightVector();
	
	auto cameraPos = camera.getPosition();
	cameraPos += cameraForward * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_forwardMult;
	cameraPos += cameraRight * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_rightMult;
	cameraPos += glm::vec3(0.0f, 0.0f, 1.0f) * m_cameraMovSpeed * deltaTime * (float)m_cameraMovDir.m_upMult;
	camera.setPosition(cameraPos);
			
	float deltaPitch = m_cameraRot.m_pitch * deltaTime * m_cameraRotSpeed;
	float deltaYaw = m_cameraRot.m_yaw * deltaTime * m_cameraRotSpeed;
	m_cameraRot.zero();
	
	auto cameraRot = camera.getRotation();
	cameraRot += glm::vec3{ 0.0f, deltaPitch, deltaYaw };
	cameraRot.y = std::clamp(cameraRot.y, -89.0f, 89.0f);
	camera.setRotation(cameraRot);
	
	if (m_attachToLight)
	{
		Renderer& renderer = Renderer::get();
		LightPoint& lightPoint = renderer.getLightPoint();
		lightPoint.setPosition(cameraPos);
	}
}
