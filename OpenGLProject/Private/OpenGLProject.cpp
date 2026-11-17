// OpenGLProject.cpp : Defines the entry point for the application.
//

#include "OpenGLProject.h"

#include <chrono>

#include "SystemBase/SystemLocator.h"
#include "Utilities/Logger.h"

#include "AssetManager/AssetManager.h"
#include "InputSystem/InputManager.h"
#include "ProjectSettings/ProjectSettings.h"
#include "Rendering/Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Scripting/ScriptManager.h"
#include "Window/WindowManager.h"


int main()
{
	auto& systemLocator = SystemLocator::Get();
	
	systemLocator.RegisterSystem<ResourceManager>();
	systemLocator.RegisterSystem<AssetManager>();
	systemLocator.RegisterSystem<FIE::IInputManager, FIE::InputManager>();
	systemLocator.RegisterSystem<FIE::WindowManager>();
	systemLocator.RegisterSystem<Renderer>();
	systemLocator.RegisterSystem<ProjectSettings>();
	systemLocator.RegisterSystem<SceneManager>();
	systemLocator.RegisterSystem<ScriptManager>();
	
	systemLocator.InitializeSystems<
		ProjectSettings,
		FIE::WindowManager,
		FIE::IInputManager,
		Renderer,
		SceneManager,
		ScriptManager
		>();
	
	// TODO: Remove this thing from here
	// --------------------------------
	auto* system = SystemLocator::Get().GetSystem<FIE::WindowManager>();
	if (!system)
	{
		LOG_ERROR("Main: Failed to get window ref");
		return 1;
	}
	auto& windowManager = *static_cast<FIE::WindowManager*>(system);
	// --------------------------------

	auto prevTime = std::chrono::high_resolution_clock::now(); 
	while (!windowManager.GetWindow()->isWindowShouldClose())
	{
		auto currTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
		prevTime = currTime;
		
		LOG_INFO("FPS: {}", 1.0f / deltaTime);
		
		systemLocator.UpdateSystems< 
			FIE::IInputManager,
			ScriptManager,
			SceneManager,
			Renderer,
			FIE::WindowManager
			>(deltaTime);
	}
	
	systemLocator.DeinitializeSystems<
		ScriptManager,
		SceneManager,
		ProjectSettings,
		FIE::IInputManager,
		Renderer,
		ResourceManager,
		FIE::WindowManager
		>();
	
	return 0;
}
	
