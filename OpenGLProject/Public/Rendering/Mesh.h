#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Utilities/Utilities.h"

struct MeshDesc
{
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
};

class Mesh
{
public:
    
    Mesh() = default;
    Mesh(const MeshDesc& desc);
    
    Mesh(const Mesh& other) = default;
    Mesh& operator=(const Mesh& other) = default;
    
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;
    
    ~Mesh();
    
    void setup(const MeshDesc& desc);
    void draw() const;
    
    bool isValid() const;

protected:
    
    void setupBuffers(const MeshDesc& desc);
    void destroyMesh();
    
private:

    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    
    uint32_t m_numIndices = 0;
    
    Transform m_transform;
    
};

struct MeshSpawnData
{
    MeshSpawnData() = delete;
    MeshSpawnData(const MeshSpawnData& other)
        : m_mesh(other.m_mesh)
        , m_transform(other.m_transform)
    {}
    MeshSpawnData(Mesh& mesh)
        : m_mesh(mesh)
    {}
    
    Mesh& m_mesh;
    Transform m_transform;
};