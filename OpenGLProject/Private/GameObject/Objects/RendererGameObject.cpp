#include "RendererGameObject.h"

#include "GameObject/Components/MeshComponent.h"
#include "GameObject/Components/TransformComponent.h"
#include "Rendering/Renderer.h"
#include "Scene/SceneManager.h"

void RendererGameObject::Update(float deltaTime)
{
    auto& registry = SceneManager::Get().GetRegistry();
    auto& renderer = Renderer::get();
    registry.view<TransformComponent, MeshComponent>().each(
        [&](entt::entity entity, const TransformComponent& transform, const MeshComponent& meshComponent)
        {
            GeometryDrawDesc desc = {
                meshComponent.m_meshHandle,
                meshComponent.m_materialHandles[0],
                transform.m_transform
            };
            renderer.addDraw(desc);
        });
}
