#pragma once

#include "Mesh.h"

struct MeshCollection
{
    uint32_t m_offset = 0;
    uint32_t m_count = 0;
};

struct MeshLoadDesc
{
    std::string m_meshName;
    std::string m_meshPath;
};

class MeshManager
{
    
    using MeshCollections = std::unordered_map<std::string, MeshCollection>;
    using MeshesVector = std::vector<Mesh>;
    
public:
    
    using ConstMeshIterator = MeshesVector::const_iterator;
    
    static MeshManager& get();
    
    void loadMesh(const MeshLoadDesc& loadDesc);
    bool getMeshes(const std::string& collectionName, ConstMeshIterator& begin, ConstMeshIterator& end) const;
    void registerMesh(const std::string& meshName, const MeshDesc& meshDesc);
    void clearAllMeshes();
    
private:

    MeshCollections m_meshCollections;
    MeshesVector m_meshes;
    
};
