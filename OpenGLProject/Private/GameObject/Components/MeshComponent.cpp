#include "GameObject/Components/MeshComponent.h"

#include "GameObject/ComponentAllocator.h"
#include "GameObject/ComponentFactory.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"

void Deserialize(
    MeshComponent& component,
    const nlohmann::json& data,
    const std::unordered_map<uint32_t, entt::entity>&
    )
{
    ResourceManager& resourceManager = ResourceManager::Get();

    if (!data.contains("mesh") || !data["mesh"].is_string())
    {
        LOG_ERROR("Mesh component must contain a mesh path");
    }
    else
    {
        component.m_meshHandle = resourceManager.LoadResource<MeshResource>(data["mesh"]);
    }

    if (!data.contains("materials"))
    {
        return;
    }
    if (!data["materials"].is_array())
    {
        LOG_ERROR("Mesh component materials must be an array");
        return;
    }

    for (const auto& material : data["materials"])
    {
        if (!material.is_string())
        {
            LOG_ERROR("Mesh component has an invalid material path");
            continue;
        }
        component.m_materialHandles.push_back(resourceManager.LoadResource<MaterialResource>(material));
    }
}

REGISTER_COMPONENT_FACTORY(MeshComponent, Deserialize)
