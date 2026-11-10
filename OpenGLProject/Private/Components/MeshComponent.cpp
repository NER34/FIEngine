#include "Components/MeshComponent.h"

#include "AssetManager/AssetManager.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Utilities/ParsingHelpers.h"

DEFINE_COMPONENT_DESERIALIZE_FUNC(Deserialize, MeshComponent)
{
    auto& component = handle.m_registry.get<MeshComponent>(handle.m_entity);
    
    if (!HasStringField(data, "mesh"))
    {
        LOG_ERROR("Mesh component must contain a mesh path");
    }
    else
    {
        const std::string& meshPath = data["mesh"];
        if (!meshPath.empty() && !AssetManager::Get().Load<MeshResource>(meshPath, component.m_meshHandle))
        {
            LOG_WARN("Mesh component failed to load mesh {}", meshPath);
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
    auto& to = handle_to.m_registry.get<MeshComponent>(handle_to.m_entity);
    const auto& from = handle_from.m_registry.get<MeshComponent>(handle_from.m_entity);
    
    to.m_meshHandle = from.m_meshHandle;
    to.m_materialHandles = from.m_materialHandles;
}

REGISTER_COMPONENT_FACTORY(MeshComponent, MeshComponent::Deserialize, MeshComponent::Copy)
