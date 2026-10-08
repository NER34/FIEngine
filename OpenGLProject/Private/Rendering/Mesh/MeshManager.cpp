#include "Rendering/Mesh/MeshManager.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "Utilities/Utilities.h"

MeshManager& MeshManager::get()
{
    static MeshManager instance;
    return instance;
}

void MeshManager::loadMesh(const MeshLoadDesc& desc)
{
    if (m_meshCollections.contains(desc.m_meshName))
    {
        return;
    }
    
    Assimp::Importer importer;
    std::string fullPath = (getResourcesPath() / desc.m_meshPath).string();
    auto* scene = importer.ReadFile(
        fullPath.c_str(), 
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
        );
    
    MeshCollection meshCollection;
    meshCollection.m_offset = static_cast<uint32_t>(m_meshes.size());
    meshCollection.m_count = static_cast<uint32_t>(scene->mNumMeshes);
    m_meshCollections.insert({desc.m_meshName, meshCollection});
    
    for (uint32_t i = 0; i < meshCollection.m_count; i++)
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

bool MeshManager::getMeshes(const std::string& collectionName, ConstMeshIterator& begin, ConstMeshIterator& end) const
{
    auto element = m_meshCollections.find(collectionName);
    if (element != m_meshCollections.end())
    {
        auto& [offset, count] = element->second;
        begin = m_meshes.begin() + offset;
        end = begin + count;
    }
    return false;
}

void MeshManager::registerMesh(const std::string& meshName, const MeshDesc& meshDesc)
{
    MeshCollection meshCollection;
    meshCollection.m_offset = static_cast<uint32_t>(m_meshes.size());
    meshCollection.m_count = static_cast<uint32_t>(1);
    m_meshCollections.insert({meshName, meshCollection});
    
    m_meshes.emplace_back(meshDesc);
}

void MeshManager::clearAllMeshes()
{
    m_meshCollections.clear();
    m_meshes.clear();
}
