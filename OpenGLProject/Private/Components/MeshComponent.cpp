#include "Components/MeshComponent.h"

#include "AssetManager/AssetManager.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, MeshComponent)
{
    if (!data.contains("mesh") || !data["mesh"].is_string())
    {
        LOG_ERROR("Mesh component must contain a mesh path");
    }
    else
    {
        if (!AssetManager::Get().Load<MeshResource>(data["mesh"], component.m_meshHandle))
        {
            LOG_ERROR("Mesh component failed to load mesh {}", data["mesh"].get<std::string>());
        }
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
        ResourceHandle<MaterialResource> materialHandle;
        if (!AssetManager::Get().Load<MaterialResource>(material, materialHandle))
        {
            LOG_ERROR("Mesh component failed to load material {}", material.get<std::string>());
        }
        component.m_materialHandles.push_back(materialHandle);
    }
}

DEFINE_COMPONENT_COPY_FUNC(Copy, MeshComponent)
{
    to.m_meshHandle = from.m_meshHandle;
    to.m_materialHandles = from.m_materialHandles;
}

REGISTER_COMPONENT_FACTORY(MeshComponent, MeshComponent::Deserialize, MeshComponent::Copy)
