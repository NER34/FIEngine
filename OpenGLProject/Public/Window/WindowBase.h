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
		DECLARE_MULTI_DISPATCHER(TOnWindowFullScreen, bool)
		DECLARE_MULTI_DISPATCHER(TOnWindowCloseRequested)

		TOnWindowResized			m_onResized;
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

		virtual void update() = 0;

	protected:

		void updateWindowSize(uint32_t width, uint32_t height);
		void updateWindowFullScreen(bool fullScreen);
		virtual void requestWindowClose();

	public:

		WindowCallbacks m_windowCallbacks;

	private:

		WindowDesc m_props;

	};
}