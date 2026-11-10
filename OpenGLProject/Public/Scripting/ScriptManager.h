#pragma once

#include "SystemBase/SystemLocator.h"

class ScriptManager : public ISystemBase
{
public:
    
    static ScriptManager& Get();
    
    void Initialize() override;
    void Update(float deltaTime) override;
    void Deinitialize() override;

private:

    void BeginPlayPendingScripts();

};
