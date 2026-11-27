#pragma once

#include "InputAdapterGLFW.h"
#include "GLFW/glfw3.h"

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
		
		bool isWindowShouldClose() const override;

		void setWindowSize(uint32_t width, uint32_t height) override;
		void setWindowFullScreen(bool fullScreen) override;
		void requestWindowClose() override;
		
		
		GLFWwindow* getGLFWWindow() const;

	private:
		
		void SetWindowFullscreen_Internal(bool fullScreen);

		static void onUpdateWindowSize(GLFWwindow* window, int width, int height);
		static void onUpdateFramebufferSize(GLFWwindow* window, int width, int height);
		static void onWindowCloseRequest(GLFWwindow* window);

	private:

		GLFWwindow* m_window = nullptr;
		
		int m_savedH = 0;
		int m_savedW = 0;
		int m_savedX = 0;
		int m_savedY = 0;
		bool m_savedMaximized = false;
	};
}