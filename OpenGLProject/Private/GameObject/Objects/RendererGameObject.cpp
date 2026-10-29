#include "GameObject/Objects/RendererGameObject.h"

#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Rendering/Renderer.h"
#include "Scene/SceneManager.h"

void RendererGameObject::Update(float deltaTime)
{
    auto& renderer = Renderer::get();
    SceneManager::Get().View<TransformComponent, MeshComponent>().each(
        [&](entt::entity entity, const TransformComponent& transform, const MeshComponent& meshComponent)
        {
            if (!meshComponent.m_meshHandle.IsValid() || meshComponent.m_materialHandles.empty() || !meshComponent.m_materialHandles[0].IsValid())
            {
                return;
            }
            
            GeometryDrawDesc desc = {
                meshComponent.m_meshHandle,
                meshComponent.m_materialHandles[0],
                transform.m_transform
            };
            renderer.addDraw(desc);
        });
}
