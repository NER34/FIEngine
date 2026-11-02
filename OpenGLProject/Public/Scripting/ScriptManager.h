#pragma once

class ScriptManager
{
public:
    
    static ScriptManager& Get();
    
    void Initialize();
    void Update(float deltaTime);
    void Deinitialize();

private:

    void BeginPlayPendingScripts();

};
