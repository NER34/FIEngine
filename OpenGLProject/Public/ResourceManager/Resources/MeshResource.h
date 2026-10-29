#pragma once

#include "glad/glad.h"

#include "AssetManager/AssetFactory.h"
#include "ResourceManager/ResourceBase.h"
#include "Utilities/Utilities.h"

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_UV_LOCATION 2

struct MeshDesc
{
    uint32_t m_numVerts;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_uvCoords;
    std::vector<uint32_t> m_indices;
};

class MeshResource : public ResourceBase
{
public:
    
    MeshResource() = default;
    ~MeshResource() override;
    
    MeshResource(const MeshResource& other) = delete;
    MeshResource& operator=(const MeshResource& other) = delete;
    
    void Free() override;
    
    void AddSubMesh(const MeshDesc& desc);
    
    size_t GetSubMeshCount() const;
    void DrawSubMesh(size_t index) const;
    void Draw() const;
    
    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, MeshResource);
    
private:
    
    struct SubMesh
    {
        GLuint m_VAO = 0;
        GLuint m_VBO_Pos = 0;
        GLuint m_VBO_UV = 0;
        GLuint m_VBO_Norm = 0;
        GLuint m_EBO = 0;
        
        uint32_t m_numIndices = 0;
    };
    
    static SubMesh CreateSubMesh(const MeshDesc& desc);
    static void DestroySubMesh(SubMesh& subMesh);
    
    std::vector<SubMesh> m_subMeshes;
    
};
