#include "ResourceManager/Resources/MeshResource.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

MeshResource::~MeshResource()
{
    MeshResource::Free();
}

void MeshResource::Reload()
{
    Free();
    
    Assimp::Importer importer;
    std::string fullPath = (getResourcesPath() / GetFilePath()).string();
    auto* scene = importer.ReadFile(
        fullPath.c_str(),
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
    );

    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* aimesh = scene->mMeshes[i];
        MeshDesc meshDesc;
        meshDesc.m_numVerts = aimesh->mNumVertices;
        meshDesc.m_vertices.reserve(aimesh->mNumVertices);
        meshDesc.m_normals.reserve(aimesh->mNumVertices);
        meshDesc.m_uvCoords.reserve(aimesh->mNumVertices);

        bool hasUV = aimesh->HasTextureCoords(0);
        aiVector3D zeroUV(0.0f, 0.0f, 0.0f);

        for (uint32_t j = 0; j < aimesh->mNumVertices; j++)
        {
            auto& vertex = aimesh->mVertices[j];
            meshDesc.m_vertices.push_back(vertex.x);
            meshDesc.m_vertices.push_back(vertex.y);
            meshDesc.m_vertices.push_back(vertex.z);

            auto& normal = aimesh->mNormals[j];
            meshDesc.m_normals.push_back(normal.x);
            meshDesc.m_normals.push_back(normal.y);
            meshDesc.m_normals.push_back(normal.z);

            auto& uvCoord = hasUV ? aimesh->mTextureCoords[0][j] : zeroUV;
            meshDesc.m_uvCoords.push_back(uvCoord.x);
            meshDesc.m_uvCoords.push_back(uvCoord.y);
        }

        for (uint32_t j = 0; j < aimesh->mNumFaces; j++)
        {
            auto& face = aimesh->mFaces[j];
            assert(face.mNumIndices == 3);
            meshDesc.m_indices.push_back(face.mIndices[0]);
            meshDesc.m_indices.push_back(face.mIndices[1]);
            meshDesc.m_indices.push_back(face.mIndices[2]);
        }
        m_meshes.emplace_back(meshDesc);
    }
    importer.FreeScene();
}

void MeshResource::Free()
{
    m_meshes.clear();
}

const std::vector<Mesh>& MeshResource::GetMeshes() const
{
    return m_meshes;
}
