#pragma once

class GameObject
{
public:
    
    virtual void beginPlay() {}
    virtual void update(float deltaTime) {}
    virtual void endPlay() {}
    
};
