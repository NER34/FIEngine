#pragma once

class ScriptBase
{
public:
    
    virtual ~ScriptBase() = default;
    
    virtual void BeginPlay() {}
    virtual void Update(float deltaTime) {}
    virtual void EndPlay() {}
    
};
