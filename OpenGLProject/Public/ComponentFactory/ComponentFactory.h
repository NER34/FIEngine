#pragma once

#include <functional>

#include "entt.h"
#include "json.h"

#include "Registry/Registry.h"
#include "ComponentFactoryStructs.h"

class IComponentFactory;

class ComponentRegistry : public Registry<std::string, IComponentFactory>
{
public:
    static ComponentRegistry& Get()
    {
        static ComponentRegistry s_instance;
        return s_instance;
    }
};

class IComponentFactory
{
public:
    
    virtual ~IComponentFactory() = default;
    
    virtual void CreateFromAsset(ComponentHandle handle, const nlohmann::json& data, const EntityIDMapping& mapping) const = 0;
    virtual void CloneComponent(ConstComponentHandle from, ComponentHandle to, const EntityIDMapping& mapping) const = 0;
    virtual void CloneAll(const entt::registry& from, entt::registry& to, const EntityIDMapping& mapping) const = 0;

};

template <typename TComponent>
class ComponentFactory : public IComponentFactory
{
    using DeserializeFunc = std::function<void(ComponentHandle, const nlohmann::json&, const EntityIDMapping&)>;
    using CopyFunc = std::function<void(ConstComponentHandle, ComponentHandle, const EntityIDMapping&)>;

public:
    
    ComponentFactory(DeserializeFunc deserialize, CopyFunc copy) 
        : m_deserialize(deserialize) 
        , m_copy(copy)
    {}
    
    void CreateFromAsset(ComponentHandle handle, const nlohmann::json& data, const EntityIDMapping& mapping) const final
    {
        handle.m_registry.emplace_or_replace<TComponent>(handle.m_entity);
        m_deserialize(handle, data, mapping);
    }
    
    void CloneComponent(ConstComponentHandle from, ComponentHandle to, const EntityIDMapping& mapping) const final
    {
        to.m_registry.emplace<TComponent>(to.m_entity);
        m_copy(from, to, mapping);
    }
    
    void CloneAll(const entt::registry& from, entt::registry& to, const EntityIDMapping& mapping) const final
    {
        from.view<TComponent>().each([&](entt::entity entity, const TComponent& component)
        {
            ConstComponentHandle compHandle_from = { from, entity };
            ComponentHandle compHandle_to = { to, mapping.Get(entity) };
            CloneComponent(compHandle_from, compHandle_to, mapping);
        });
    }

private:
    
    DeserializeFunc m_deserialize;  
    CopyFunc        m_copy;  
    static bool     s_registered;
    
};


#define DECLARE_COMPONENT_DESERIALIZE_FUNC(FuncName, TComponent) \
    static void FuncName(ComponentHandle handle, const nlohmann::json& data, const EntityIDMapping& mapping)

#define DEFINE_COMPONENT_DESERIALIZE_FUNC(FuncName, TComponent) \
    void TComponent::FuncName(ComponentHandle handle, const nlohmann::json& data, const EntityIDMapping& mapping)

#define DECLARE_COMPONENT_COPY_FUNC(FuncName, TComponent) \
    static void FuncName(ConstComponentHandle handle_from, ComponentHandle handle_to, const EntityIDMapping& mapping)
    
#define DEFINE_COMPONENT_COPY_FUNC(FuncName, TComponent) \
    void TComponent::FuncName(ConstComponentHandle handle_from, ComponentHandle handle_to, const EntityIDMapping& mapping)


#define COMPONENT_FACTORY_REGISTER_VAR(TComponent) template<> bool ComponentFactory<TComponent>::s_registered
#define COMPONENT_FACTORY_REGISTER_FUNC(TComponent) ComponentRegistry::Get().RegisterObject<ComponentFactory<TComponent>>

#define REGISTER_COMPONENT_FACTORY(TComponent, DeserializeFunc, CopyFunc) \
    COMPONENT_FACTORY_REGISTER_VAR(TComponent) = COMPONENT_FACTORY_REGISTER_FUNC(TComponent) \
    ( \
        #TComponent, DeserializeFunc, CopyFunc \
    );
