#pragma once

class GameObject
{
public:
    
    virtual void BeginPlay() {}
    virtual void Update(float deltaTime) {}
    virtual void EndPlay() {}
    
};
