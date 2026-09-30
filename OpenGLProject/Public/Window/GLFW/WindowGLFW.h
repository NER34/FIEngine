#pragma once

#include "InputAdapterGLFW.h"

#include "Window/WindowBase.h"

namespace FIE
{

	class InputAdapterGLFW;

	class WindowGLFW : public IWindow
	{
	public:

		static uint32_t s_windowsCount;

		static void glfwErrorCallback(int error, const char* description);

		static bool initializeGLFW();
		static void deinitializeGLFW();

		WindowGLFW();
		WindowGLFW(const WindowDesc& props);
		~WindowGLFW() override;

		void initialize();
		void update() override;

		void setWindowSize(uint32_t width, uint32_t height) override;
		void setWindowFullScreen(bool fullScreen) override;
		
		GLFWwindow* getGLFWWindow() const;

	private:

		static void onUpdateWindowSize(GLFWwindow* window, int width, int height);
		static void onWindowCloseRequest(GLFWwindow* window);

	private:

		GLFWwindow* m_window = nullptr;

	};
}