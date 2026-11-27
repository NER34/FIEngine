#include "Window/WindowBase.h"

#include <algorithm>

namespace FIE
{
	IWindow::IWindow(const WindowDesc& props)
		: m_props(props)
	{}

	void IWindow::setWindowSize(uint32_t width, uint32_t height)
	{
		updateWindowSize(width, height);
	}

	void IWindow::setWindowFullScreen(bool fullScreen)
	{
		if (fullScreen != m_props.m_fullScreen)
			updateWindowFullScreen(fullScreen);
	}

	void IWindow::updateWindowSize(uint32_t width, uint32_t height)
	{
		m_props.m_width = width;
		m_props.m_height = height;
		m_windowCallbacks.m_onResized.Broadcast(width, height);
	}

	void IWindow::updateFramebufferSize(uint32_t width, uint32_t height)
	{
		m_framebufferWidth = width;
		m_framebufferHeight = height;
		m_windowCallbacks.m_onFramebufferResized.Broadcast(width, height);
	}

	void IWindow::updateWindowFullScreen(bool fullScreen)
	{
		m_props.m_fullScreen = fullScreen;
		m_windowCallbacks.m_onFullScreen.Broadcast(fullScreen);
	}

	void IWindow::requestWindowClose()
	{
		m_windowCallbacks.m_onCloseRequest.Broadcast();
	}

	const WindowDesc& IWindow::getWindowDesc() const
	{
		return m_props;
	}

	uint32_t IWindow::getFramebufferWidth() const
	{
		return m_framebufferWidth;
	}

	uint32_t IWindow::getFramebufferHeight() const
	{
		return m_framebufferHeight;
	}

	bool IWindow::isMinimized() const
	{
		return m_framebufferWidth == 0 || m_framebufferHeight == 0;
	}

	float IWindow::GetAspectRatio() const
	{
		// Window becomes 0x0 when minimized - avoid NaN/inf reaching glm::perspective
		if (isMinimized())
		{
			return 1.0f;
		}
		return (float)m_framebufferWidth / (float)m_framebufferHeight;
	}
}
