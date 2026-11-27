#pragma once

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MeshResource.h"
#include "ResourceManager/Resources/MaterialResource.h"
#include "ComponentFactory/ComponentFactory.h"

class MeshComponent
{
public:
    
    ResourceHandle<MeshResource> m_meshHandle;
    std::vector<ResourceHandle<MaterialResource>> m_materialHandles;
    
    DECLARE_COMPONENT_DESERIALIZE_FUNC(Deserialize, MeshComponent);
    DECLARE_COMPONENT_COPY_FUNC(Copy, MeshComponent);
    
};
