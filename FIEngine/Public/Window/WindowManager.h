#pragma once

#include "WindowBase.h"
#include "SystemBase/SystemLocator.h"

namespace FIE
{
    class IWindow;

    class WindowManager : public ISystemBase
    {
    public:
        
        void Initialize() override;
        void Update(float deltaTime) override;
        void Deinitialize() override;
        
        const std::unique_ptr<IWindow>& GetWindow() const;
        
    private:
        
        std::unique_ptr<IWindow> m_window;
        
    };    
    
}
