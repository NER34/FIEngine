#pragma once

#include <string>

#include "EventDispatcher/Dispatcher.h"

namespace FIE
{
	struct WindowDesc
	{
		std::string m_title	= "FIEngine New Window";
		uint32_t m_width	= 1280;
		uint32_t m_height	= 720;
		bool m_fullScreen	= false;
	};

	struct WindowCallbacks
	{
		DECLARE_MULTI_DISPATCHER(TOnWindowResized, uint32_t, uint32_t)
		DECLARE_MULTI_DISPATCHER(TOnFramebufferResized, uint32_t, uint32_t)
		DECLARE_MULTI_DISPATCHER(TOnWindowFullScreen, bool)
		DECLARE_MULTI_DISPATCHER(TOnWindowCloseRequested)

		// Window size in screen coordinates
		TOnWindowResized			m_onResized;
		// Drawable surface size in pixels (use this for glViewport)
		TOnFramebufferResized		m_onFramebufferResized;
		TOnWindowFullScreen			m_onFullScreen;
		TOnWindowCloseRequested		m_onCloseRequest;
	};

	class IWindow
	{
	public:

		IWindow() = default;
		IWindow(const WindowDesc& props);
		virtual ~IWindow() = default;

		virtual void setWindowSize(uint32_t width, uint32_t height);
		virtual void setWindowFullScreen(bool fullScreen);
		virtual bool isWindowShouldClose() const = 0;

		const WindowDesc& getWindowDesc() const;
		uint32_t getFramebufferWidth() const;
		uint32_t getFramebufferHeight() const;
		bool isMinimized() const;
		float GetAspectRatio() const;

		virtual void update() = 0;
		virtual void requestWindowClose();

	protected:

		void updateWindowSize(uint32_t width, uint32_t height);
		void updateFramebufferSize(uint32_t width, uint32_t height);
		void updateWindowFullScreen(bool fullScreen);

	public:

		WindowCallbacks m_windowCallbacks;

	private:

		WindowDesc m_props;
		uint32_t m_framebufferWidth		= m_props.m_width;
		uint32_t m_framebufferHeight	= m_props.m_height;

	};
}