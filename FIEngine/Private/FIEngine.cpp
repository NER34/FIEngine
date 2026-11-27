// FIEngine.cpp : Defines the entry point for the application.
//

#include "FIEngine.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <string>

#include "SystemBase/SystemLocator.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

#include "AssetManager/AssetManager.h"
#include "easy/profiler.h"
#include "InputSystem/InputManager.h"
#include "ProjectSettings/ProjectSettings.h"
#include "Rendering/Renderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Scripting/ScriptManager.h"
#include "Window/WindowManager.h"


int main()
{      
	EASY_PROFILER_ENABLE         // start collecting                                                   
	EASY_MAIN_THREAD             // name this thread "Main" in the viewer                              
	profiler::startListen();     // let the GUI connect live (port 28077)
	
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
	
	// TODO: Remove this thing from here if there will be a chance
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
		EASY_BLOCK("Frame", profiler::colors::Magenta)
		
		auto currTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currTime - prevTime).count();
		prevTime = currTime;
		
		//LOG_INFO("FPS: {}", 1.0f / deltaTime);
		
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
	
	{
		const std::time_t now = std::time(nullptr);
		std::tm localTime{};
#ifdef _WIN32
		localtime_s(&localTime, &now);
#else
		localtime_r(&now, &localTime);
#endif
		char timestamp[16];
		std::strftime(timestamp, sizeof(timestamp), "%d%m%y_%H%M%S", &localTime);

		const std::filesystem::path savedDir = GetSavedDirectoryPath();
		std::error_code ec;
		std::filesystem::create_directories(savedDir, ec);

		const std::filesystem::path capturePath = savedDir / ("capture_" + std::string(timestamp) + ".prof");
		profiler::dumpBlocksToFile(capturePath.string().c_str());
	}
	
	return 0;
}
	
