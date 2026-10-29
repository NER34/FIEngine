#include "ResourceManager/Resources/MeshResource.h"

#include <cassert>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "Rendering/Validation.h"
#include "Utilities/Logger.h"

MeshResource::~MeshResource()
{
    MeshResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, MeshResource)
{
    resource.Free();

    if (!data.contains("path") || !data["path"].is_string())
    {
        LOG_ERROR("Invalid mesh asset file: path is missing");
        return false;
    }
    
    Assimp::Importer importer;
    std::string fullPath = (GetResourcesPath() / data["path"].get<std::string>()).string();
    auto* scene = importer.ReadFile(
        fullPath.c_str(),
        aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
    );
    
    if (!scene)
    {
        LOG_ERROR("Failed to load mesh: {} ({})", fullPath, importer.GetErrorString());
        return false;
    }

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
        resource.AddSubMesh(meshDesc);
    }
    importer.FreeScene();
    return true;
}

void MeshResource::Free()
{
    for (auto& subMesh : m_subMeshes)
    {
        DestroySubMesh(subMesh);
    }
    m_subMeshes.clear();
}

void MeshResource::AddSubMesh(const MeshDesc& desc)
{
    m_subMeshes.push_back(CreateSubMesh(desc));
}

size_t MeshResource::GetSubMeshCount() const
{
    return m_subMeshes.size();
}

void MeshResource::DrawSubMesh(size_t index) const
{
    assert(index < m_subMeshes.size());
    const auto& subMesh = m_subMeshes[index];
    
    CHECK_OGL_ERR(glBindVertexArray(subMesh.m_VAO))
    
    CHECK_OGL_ERR(glDrawElements(
        GL_TRIANGLES, 
        static_cast<GLsizei>(subMesh.m_numIndices), 
        GL_UNSIGNED_INT, 
        0
        ))
    
    CHECK_OGL_ERR(glBindVertexArray(0))
}

void MeshResource::Draw() const
{
    for (size_t i = 0; i < m_subMeshes.size(); i++)
    {
        DrawSubMesh(i);
    }
}

MeshResource::SubMesh MeshResource::CreateSubMesh(const MeshDesc& desc)
{
    SubMesh subMesh;
    
    CHECK_OGL_ERR(glGenVertexArrays(1, &subMesh.m_VAO))
    CHECK_OGL_ERR(glGenBuffers(1, &subMesh.m_VBO_Pos))
    CHECK_OGL_ERR(glGenBuffers(1, &subMesh.m_VBO_Norm))
    CHECK_OGL_ERR(glGenBuffers(1, &subMesh.m_VBO_UV))
    CHECK_OGL_ERR(glGenBuffers(1, &subMesh.m_EBO))

    CHECK_OGL_ERR(glBindVertexArray(subMesh.m_VAO))
    
    auto setupVBO = [numVerts = desc.m_numVerts](GLenum location, GLsizei numFloats, GLuint vbo, const void* data)
    {
        GLsizei bufSize = numVerts * numFloats * sizeof(GLfloat);
        CHECK_OGL_ERR(glBindBuffer(GL_ARRAY_BUFFER, vbo))
        CHECK_OGL_ERR(glBufferData(GL_ARRAY_BUFFER, bufSize, data, GL_STATIC_DRAW))
        CHECK_OGL_ERR(glEnableVertexAttribArray(location))
        CHECK_OGL_ERR(glVertexAttribPointer(location, numFloats, GL_FLOAT, GL_FALSE, numFloats * sizeof(float), (void*)0))	
    };
    
    setupVBO(POSITION_LOCATION, 3, subMesh.m_VBO_Pos, desc.m_vertices.data());
    setupVBO(NORMAL_LOCATION, 3, subMesh.m_VBO_Norm, desc.m_normals.data());
    setupVBO(TEX_UV_LOCATION, 2, subMesh.m_VBO_UV, desc.m_uvCoords.data());
    
    CHECK_OGL_ERR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh.m_EBO))
    GLsizei bufSize = desc.m_indices.size() * sizeof(GLuint);
    CHECK_OGL_ERR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, desc.m_indices.data(), GL_STATIC_DRAW))
    subMesh.m_numIndices = desc.m_indices.size();
    
    CHECK_OGL_ERR(glBindVertexArray(0))
    CHECK_OGL_ERR(glBindBuffer(GL_ARRAY_BUFFER, 0))
    CHECK_OGL_ERR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
    
    return subMesh;
}

void MeshResource::DestroySubMesh(SubMesh& subMesh)
{
    CHECK_OGL_ERR(glDeleteVertexArrays(1, &subMesh.m_VAO))
    CHECK_OGL_ERR(glDeleteBuffers(1, &subMesh.m_VBO_Pos))
    CHECK_OGL_ERR(glDeleteBuffers(1, &subMesh.m_VBO_UV))
    CHECK_OGL_ERR(glDeleteBuffers(1, &subMesh.m_VBO_Norm))
    CHECK_OGL_ERR(glDeleteBuffers(1, &subMesh.m_EBO))
    
    subMesh = SubMesh();
}

REGISTER_ASSET_FACTORY(MeshResource, MeshResource::Deserialize)
