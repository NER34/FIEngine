#include "Window/WindowManager.h"

#include "Window/WindowBase.h"
#include "Window/GLFW/WindowGLFW.h"
#include "ProjectSettings/ProjectSettings.h"
#include "easy/profiler.h"


namespace FIE
{
    void WindowManager::Initialize()
    {
        EASY_FUNCTION();

        WindowDesc windowDesc;
        if (auto* system = SystemLocator::Get().GetSystem<ProjectSettings>())
        {
            ProjectSettings& projectSettings = *static_cast<ProjectSettings*>(system);
            windowDesc.m_title = projectSettings->m_windowName;
            windowDesc.m_width = projectSettings->m_windowWidth;
            windowDesc.m_height = projectSettings->m_windowHeight;
        }
        m_window = std::make_unique<WindowGLFW>(windowDesc);
    }
    
    void WindowManager::Update(float deltaTime)
    {
        EASY_FUNCTION();

        m_window->update();
    }
    
    void WindowManager::Deinitialize()
    {
        EASY_FUNCTION();

        m_window.reset();
    }

    const std::unique_ptr<IWindow>& WindowManager::GetWindow() const
    {
        return m_window;
    }
}
