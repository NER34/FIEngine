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
		updateWindowFullScreen(fullScreen);
	}

	void IWindow::updateWindowSize(uint32_t width, uint32_t height)
	{
		m_props.m_width = width;
		m_props.m_height = height;
		m_windowCallbacks.m_onResized.Broadcast(width, height);
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
}
