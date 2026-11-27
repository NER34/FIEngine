
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
			glfwWindowHint(GLFW_SAMPLES, 4);
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
		
		if (props.m_fullScreen)
		{
			SetWindowFullscreen_Internal(true);
		}

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
	}

	void WindowGLFW::setWindowFullScreen(bool fullScreen)
	{
		if (fullScreen != getWindowDesc().m_fullScreen)
		{
			SetWindowFullscreen_Internal(fullScreen);
			IWindow::setWindowFullScreen(fullScreen);
		}
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

	void WindowGLFW::SetWindowFullscreen_Internal(bool fullScreen)
	{	
		if (fullScreen)
		{
			glfwGetWindowPos(m_window, &m_savedX, &m_savedY);
			glfwGetWindowSize(m_window, &m_savedW, &m_savedH);
			m_savedMaximized = glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED) == GLFW_TRUE;
					
			// TODO: Find monitor nearest to the window
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			if (!monitor)
			{
				LOG_ERROR("No monitor for fullscreen"); 
				return;
			}
			
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowAttrib(m_window, GLFW_AUTO_ICONIFY, GLFW_FALSE);
			glfwSetWindowMonitor(
				m_window, monitor, 
				0, 0, 
				mode->width, mode->height, 
				mode->refreshRate
				);
					
		}
		else
		{
			glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowAttrib(m_window, GLFW_AUTO_ICONIFY, GLFW_TRUE);
			glfwSetWindowMonitor(
				m_window, nullptr, 
				m_savedX, m_savedY, 
				m_savedW, m_savedH, 
				GLFW_DONT_CARE
				);
			if (m_savedMaximized)
			{
				glfwMaximizeWindow(m_window);
			}
		}
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