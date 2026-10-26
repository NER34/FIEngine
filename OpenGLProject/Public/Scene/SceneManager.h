#pragma once

#include <entt.h>

class SceneManager
{
public:
    
    static SceneManager& Get();
    entt::registry& GetRegistry();
    
private:
    
    entt::registry m_registry;
    
};
