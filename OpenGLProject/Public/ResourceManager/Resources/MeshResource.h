#pragma once

#include "Rendering/Mesh.h"
#include "ResourceManager/ResourceBase.h"

class MeshResource : public ResourceBase
{
public:
    
    MeshResource() = default;
    ~MeshResource() override;
    
    void Reload() override;
    void Free() override;
    
    void AddMesh(Mesh&& mesh);
    
    const std::vector<Mesh>& GetMeshes() const;
    std::vector<Mesh>& GetMeshes();
    
private:
    
    std::vector<Mesh> m_meshes;
    
};
