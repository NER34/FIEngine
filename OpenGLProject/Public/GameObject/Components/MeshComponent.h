#pragma once

#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/MeshResource.h"
#include "ResourceManager/Resources/MaterialResource.h"

class MeshComponent
{
public:
    ResourceHandle<MeshResource> m_meshHandle;
    std::vector<ResourceHandle<MaterialResource>> m_materialHandles;
};
