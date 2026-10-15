// OpenGLProject.cpp : Defines the entry point for the application.
//

#include "OpenGLProject.h"

#include <filesystem>

#include "GameObject/GameObjectManager.h"
#include "GameObject/Objects/CameraGameObject.h"
#include "GameObject/Objects/MeshGameObject.h"
#include "GameObject/Objects/PlainGameObject.h"
#include "glad/glad.h"

#include "InputSystem/InputContext.h"
#include "InputSystem/InputManager.h"
#include "Rendering/Camera.h"
#include "Rendering/Renderer.h"
#include "Rendering/Mesh/MeshManager.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Window/GLFW/WindowGLFW.h"

int main()
{
	FIE::WindowDesc windowDesc;
	windowDesc.m_title = "OpenGL Project";
	FIE::WindowGLFW window(windowDesc);
	
	FIE::IInputManager& inputManager = FIE::IInputManager::Get();
	FIE::InputContext inputContext;
	inputManager.setInputContext(&inputContext);
	
	inputContext.assignHandle(FIE::KEY_ESCAPE, [&window](FIE::InputIDs, float)
	{
		window.requestWindowClose();
	});
	
	// Load GLAD and init OpenGL
	CameraDesc cameraDesc;
	cameraDesc.m_aspectRatio = (float)window.getWindowDesc().m_width / (float)window.getWindowDesc().m_height;
	
	RendererDesc rendererDesc;
	rendererDesc.m_viewportDesc = {
		0, 0,
		(GLsizei)window.getWindowDesc().m_width,
		(GLsizei)window.getWindowDesc().m_height
	};
	rendererDesc.m_cameraDesc = cameraDesc;
	
	Renderer& renderer = Renderer::get();
	renderer.initialize(rendererDesc);
	
	ResourceManager::Get().Initialize();
	
	GameObjectManager& gameObjectManager = GameObjectManager::get();
	gameObjectManager.createGameObject<CameraGameObject>();
	gameObjectManager.createGameObject<PlainGameObject>();
	gameObjectManager.createGameObject<MeshGameObject>();
	
	auto prevTime = std::chrono::high_resolution_clock::now(); 
	while (!window.isWindowShouldClose())
	{
		auto currTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
		prevTime = currTime;
		
		inputManager.update();
		gameObjectManager.update(deltaTime);
		renderer.render();
		window.update();
	}
	
	gameObjectManager.destroyAllGameObjects();
	MeshManager& meshManager = MeshManager::get();
	meshManager.clearAllMeshes();
	
	return 0;
}
