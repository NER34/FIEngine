#pragma once
#include <entt.h>
#include <json.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "Utilities/Logger.h"

class IComponentFactory;

class ComponentAllocator
{
public:
    
    static ComponentAllocator& Get();
    
    void AddComponentDeserialized(
        const std::string& name,
        entt::registry& registry,
        entt::entity entity,
        const nlohmann::json& data,
        const std::unordered_map<uint32_t, entt::entity>& entityIDMap
        );
    
    bool IsValidComponentName(const std::string& name);
    
    template<typename TFactory>
    bool RegisterComponentFactory(const std::string& name, TFactory&& factory)
    {
        if (m_componentFactories.contains(name))
        {
            LOG_ERROR("Component factory with name {} already registered", name);
            return false;
        }
        m_componentFactories.insert({ name, std::make_unique<TFactory>(std::move(factory)) });
        return true;
    }
    
private:
    
    std::unordered_map<std::string, std::unique_ptr<IComponentFactory>> m_componentFactories;
};

#define REGISTER_COMPONENT_FACTORY(TComponent, DeserializeFunc) \
    template<> bool ComponentFactory<TComponent>::s_registered = ComponentAllocator::Get().RegisterComponentFactory(#TComponent, ComponentFactory<TComponent>(DeserializeFunc));
