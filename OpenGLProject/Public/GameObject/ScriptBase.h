#pragma once

#include <cstdint>
#include <unordered_map>

#include "json.h"
#include "entt.h"

class ScriptBase
{
public:
    
    virtual ~ScriptBase();
    
    virtual void BeginPlay() {};
    virtual void Update(float deltaTime) {};
    virtual void EndPlay() {};
    
    virtual void Deserialize(const nlohmann::json& data, const std::unordered_map<uint32_t, entt::entity>& entityIDMap) {};
    virtual void Copy(const ScriptBase* other, const std::unordered_map<entt::entity, entt::entity>&) {};
    
};
