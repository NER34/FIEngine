#pragma once

#include "glad/glad.h"

#include "Utilities/Utilities.h"

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_UV_LOCATION 2

struct aiMesh;

struct MeshDesc
{
    uint32_t m_numVerts;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<float> m_uvCoords;
    std::vector<uint32_t> m_indices;
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
    void copy(const Mesh& other);
    void destroyMesh();
    void clearData();
    
private:

    GLuint m_VAO = 0;
    GLuint m_VBO_Pos = 0;
    GLuint m_VBO_UV = 0;
    GLuint m_VBO_Norm = 0;
    GLuint m_EBO = 0;
    
    uint32_t m_numIndices = 0;
    
};