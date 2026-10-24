#include "Rendering/Mesh.h"

#include "Rendering/Validation.h"
#include "Utilities/Utilities.h"

Mesh::Mesh(const MeshDesc& desc)
{
    setupBuffers(desc);
}

Mesh::Mesh(Mesh&& other) noexcept
{
    copy(other);
    other.clearData();
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        copy(other);
        other.clearData();
    }
    return *this;
}

Mesh::~Mesh()
{
    destroyMesh();
}

void Mesh::setup(const MeshDesc& desc)
{
    destroyMesh();
    setupBuffers(desc);
}

void Mesh::draw() const
{
    if (isValid())
    {
        CHECK_OGL_ERR(glBindVertexArray(m_VAO))
        
        CHECK_OGL_ERR(glDrawElements(
            GL_TRIANGLES, 
            static_cast<GLsizei>(m_numIndices), 
            GL_UNSIGNED_INT, 
            0
            ))
        
        CHECK_OGL_ERR(glBindVertexArray(0))
    }
}

bool Mesh::isValid() const
{
    return m_VAO != 0;
}

void Mesh::setupBuffers(const MeshDesc& desc)
{
    CHECK_OGL_ERR(glGenVertexArrays(1, &m_VAO))
    CHECK_OGL_ERR(glGenBuffers(1, &m_VBO_Pos))
    CHECK_OGL_ERR(glGenBuffers(1, &m_VBO_Norm))
    CHECK_OGL_ERR(glGenBuffers(1, &m_VBO_UV))
    CHECK_OGL_ERR(glGenBuffers(1, &m_EBO))

    CHECK_OGL_ERR(glBindVertexArray(m_VAO))
    
    auto setupVBO = [numVerts = desc.m_numVerts](GLenum location, GLsizei numFloats, GLuint vbo, const void* data)
    {
        GLsizei bufSize = numVerts * numFloats * sizeof(GLfloat);
        CHECK_OGL_ERR(glBindBuffer(GL_ARRAY_BUFFER, vbo))
        CHECK_OGL_ERR(glBufferData(GL_ARRAY_BUFFER, bufSize, data, GL_STATIC_DRAW))
        CHECK_OGL_ERR(glEnableVertexAttribArray(location))
        CHECK_OGL_ERR(glVertexAttribPointer(location, numFloats, GL_FLOAT, GL_FALSE, numFloats * sizeof(float), (void*)0))	
    };
    
    setupVBO(POSITION_LOCATION, 3, m_VBO_Pos, desc.m_vertices.data());
    setupVBO(NORMAL_LOCATION, 3, m_VBO_Norm, desc.m_normals.data());
    setupVBO(TEX_UV_LOCATION, 2, m_VBO_UV, desc.m_uvCoords.data());
    
    CHECK_OGL_ERR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO))
    GLsizei bufSize = desc.m_indices.size() * sizeof(GLuint);
    CHECK_OGL_ERR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, desc.m_indices.data(), GL_STATIC_DRAW))
	m_numIndices = desc.m_indices.size();
    
    CHECK_OGL_ERR(glBindVertexArray(0))
    CHECK_OGL_ERR(glBindBuffer(GL_ARRAY_BUFFER, 0))
    CHECK_OGL_ERR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0))
}

void Mesh::copy(const Mesh& other)
{
    m_VAO = other.m_VAO;
    
    m_VBO_Pos = other.m_VBO_Pos;
    m_VBO_UV = other.m_VBO_UV;
    m_VBO_Norm = other.m_VBO_Norm;
    
    m_EBO = other.m_EBO;
    m_numIndices = other.m_numIndices;
}

void Mesh::destroyMesh()
{
    if (isValid())
    {
        CHECK_OGL_ERR(glDeleteVertexArrays(1, &m_VAO))
        CHECK_OGL_ERR(glDeleteBuffers(1, &m_VBO_Pos))
        CHECK_OGL_ERR(glDeleteBuffers(1, &m_VBO_UV))
        CHECK_OGL_ERR(glDeleteBuffers(1, &m_VBO_Norm))
        CHECK_OGL_ERR(glDeleteBuffers(1, &m_EBO))
        
        clearData();
    }
}

void Mesh::clearData()
{
    m_VAO = m_VBO_Pos = m_VBO_UV = m_VBO_Norm = m_EBO = 0;
    m_numIndices = 0;
}
