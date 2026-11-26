
#include "Window/GLFW/WindowGLFW.h"
#include "Window/GLFW/InputAdapterGLFW.h"
#include "Utilities/Logger.h"

#include "GLFW/glfw3.h"

namespace FIE
{
	uint32_t WindowGLFW::s_windowsCount = 0;
	
	void WindowGLFW::glfwErrorCallback(int error, const char* description)
	{
		LOG_ERROR("GLFWErrorCallback: Error code: %d; Error msg: %s\n", error, description);
	}

	bool WindowGLFW::initializeGLFW()
	{
		if (s_windowsCount == 0)
		{
			glfwSetErrorCallback(glfwErrorCallback);
			if (glfwInit() != GLFW_TRUE)
			{
				LOG_ERROR("Failed to initialize GLFW.");
				return false;
			}
			// Define OpenGL version
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			// Tell GLFW that we are using CORE profile
			// So that means we only have the modern function
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			LOG_INFO("GLFW Initialized");
		}
		return true;
	}

	void WindowGLFW::deinitializeGLFW()
	{
		glfwTerminate();
	}

	WindowGLFW::WindowGLFW() 
		: IWindow()
	{
		initialize();
	}

	WindowGLFW::WindowGLFW(const WindowDesc& props) 
		: IWindow(props)
	{
		initialize();
	}

	WindowGLFW::~WindowGLFW()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			s_windowsCount--;
			if (s_windowsCount == 0)
			{
				deinitializeGLFW();
			}
		}
	}

	void WindowGLFW::initialize()
	{
		if (!initializeGLFW())
		{
			return;
		}

		const WindowDesc& props = getWindowDesc();
		m_window = glfwCreateWindow(
			props.m_width, props.m_height, props.m_title.c_str(), nullptr, nullptr
		);
		if (!m_window)
		{
			LOG_ERROR("Failed to create GLFW window");
			return;
		}
		s_windowsCount++;

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowSizeCallback(m_window, onUpdateWindowSize);
		glfwSetFramebufferSizeCallback(m_window, onUpdateFramebufferSize);
		glfwSetWindowCloseCallback(m_window, onWindowCloseRequest);

		// Framebuffer size can differ from window size (high-DPI), query the real one
		int fbWidth = 0, fbHeight = 0;
		glfwGetFramebufferSize(m_window, &fbWidth, &fbHeight);
		updateFramebufferSize((uint32_t)fbWidth, (uint32_t)fbHeight);

		InputAdapterGLFW::Initialize(m_window);
	}

	void WindowGLFW::update()
	{
		if (m_window)
		{
			glfwPollEvents();
			glfwSwapBuffers(m_window);
		}
	}

	bool WindowGLFW::isWindowShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void WindowGLFW::setWindowSize(uint32_t width, uint32_t height)
	{
		glfwSetWindowSize(m_window, width, height);
		IWindow::setWindowSize(width, height);
	}

	void WindowGLFW::setWindowFullScreen(bool fullScreen)
	{
		// TODO: Set window full screen mode
		IWindow::setWindowFullScreen(fullScreen);
	}

	void WindowGLFW::requestWindowClose()
	{
		glfwSetWindowShouldClose(m_window, 1);
		IWindow::requestWindowClose();
	}

	GLFWwindow* WindowGLFW::getGLFWWindow() const
	{
		return m_window;
	}

	void WindowGLFW::onUpdateWindowSize(GLFWwindow* window, int width, int height)
	{
		if (WindowGLFW* windowPtr = (WindowGLFW*)glfwGetWindowUserPointer(window))
		{
			windowPtr->updateWindowSize(width, height);
		}
	}

	void WindowGLFW::onUpdateFramebufferSize(GLFWwindow* window, int width, int height)
	{
		if (WindowGLFW* windowPtr = (WindowGLFW*)glfwGetWindowUserPointer(window))
		{
			windowPtr->updateFramebufferSize((uint32_t)width, (uint32_t)height);
		}
	}

	void WindowGLFW::onWindowCloseRequest(GLFWwindow* window)
	{
		if (WindowGLFW* windowPtr = (WindowGLFW*)glfwGetWindowUserPointer(window))
		{
			windowPtr->requestWindowClose();
		}
	}
}