#pragma once

#include "Rendering/Mesh/Mesh.h"
#include "ResourceManager/ResourceBase.h"

class MeshResource : public ResourceBase
{
public:
    
    MeshResource() = default;
    ~MeshResource() override;
    
    void Reload() override;
    void Free() override;
    
    const std::vector<Mesh>& GetMeshes() const;
    
private:
    
    std::vector<Mesh> m_meshes;
    
};
