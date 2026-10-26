#pragma once

class GameObject
{
public:
    
    virtual ~GameObject() = default;

    virtual void BeginPlay() {}
    virtual void Update(float deltaTime) {}
    virtual void EndPlay() {}
    
};
