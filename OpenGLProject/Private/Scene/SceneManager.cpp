#include "Scene/SceneManager.h"

#include "AssetManager/AssetManager.h"
#include "ComponentFactory/ComponentFactory.h"
#include "ComponentFactory/ComponentFactoryStructs.h"
#include "Components/TransformComponent.h"

SceneManager& SceneManager::Get()
{
    static SceneManager instance;
    return instance;
}

namespace
{
    const std::string g_scenePath = "Scenes/Default_NoPlane_C&L.asset";
}

void SceneManager::Initialize()
{
    m_sceneRoot = m_registry.create();
    TransformComponent& transform = m_registry.emplace<TransformComponent>(m_sceneRoot);
    transform.m_current = m_sceneRoot;
    
    if (!AssetManager::Get().Load<SceneResource>(g_scenePath, m_sceneHandle))
    {
        LOG_ERROR("Failed to load scene {}", g_scenePath);
        return;
    }
    SpawnScene(m_sceneHandle);
}

void SceneManager::Update()
{
    if (m_sceneGraphModified)
    {
        m_sceneGraphModified = false;
        auto& transforms = m_registry.storage<TransformComponent>();
        std::vector<entt::entity> order;
        order.reserve(transforms.size());
        
        std::vector<entt::entity> stack;
        stack.push_back(m_sceneRoot);
        while (!stack.empty())
        {
            entt::entity entity = stack.back();
            stack.pop_back();
            order.push_back(entity);
            
            for (auto child = transforms.get(entity).m_child; child != entt::null; child = transforms.get(child).m_neighbor_r)
            {
                stack.push_back(child);
            }
        }
        
        transforms.sort_as(order.begin(), order.end());
    }
    
    m_registry.view<TransformComponent>().each(
        [&](entt::entity entity, TransformComponent& transform)
        {
            transform.UpdateGlobalTransform(m_registry);
        });
}

void SceneManager::Deinitialize()
{
    m_registry.clear();
    m_sceneRoot = entt::null;
}

entt::entity SceneManager::GetRootEntity() const
{
    return m_sceneRoot;
}

void SceneManager::MarkSceneGraphModified()
{
    m_sceneGraphModified = true;
}

entt::entity SceneManager::SpawnScene(const ResourceHandle<SceneResource>& scene, entt::entity parent)
{
    MarkSceneGraphModified();
    
    EntityIDMapping mapping;
    //mapping.Map(entt::null, m_sceneRoot);
    for (auto [entity] : scene->m_registry.storage<entt::entity>()->each())
    {
        mapping.Map(entity, m_registry.create());
    }
    ComponentRegistry::Get().ForEachObject([&](const std::string&, const IComponentFactory& factory)
    {
        factory.CloneAll(scene->m_registry, m_registry, mapping);
    });
    
    entt::entity rootEntity = mapping.Get(scene->m_rootEntity);
    if (parent == entt::null)
    {
        parent = m_sceneRoot;
    }
    TransformComponent& sceneTransform = m_registry.get<TransformComponent>(rootEntity);
    TransformComponent& parentTransform = m_registry.get<TransformComponent>(parent);
    sceneTransform.AttachTo(parentTransform);
    
    for (const auto& [subSceneParent, subSceneTransform, subSceneHandle] : scene->m_subScenes)
    {
        auto subSceneRoot = SpawnScene(subSceneHandle, mapping.Get(subSceneParent));
        TransformComponent& subSceneTransformComp = m_registry.get<TransformComponent>(subSceneRoot);
        subSceneTransformComp.SetTransform(subSceneTransform);
    }
    
    return rootEntity;
}
