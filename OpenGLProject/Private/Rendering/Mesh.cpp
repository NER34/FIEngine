#include "Rendering/Mesh.h"

#include "Rendering/Validation.h"

Mesh::Mesh(const MeshDesc& desc)
    : m_numIndices(static_cast<uint32_t>(desc.m_indices.size()))
{
    setupBuffers(desc);
}

Mesh::Mesh(Mesh&& other) noexcept
{
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_EBO = other.m_EBO;
        
        other.m_VAO = other.m_VBO = other.m_EBO = 0;
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
    m_numIndices = static_cast<uint32_t>(desc.m_indices.size());
    setupBuffers(desc);
}

void Mesh::draw() const
{
    if (isValid())
    {
        glBindVertexArray(m_VAO);
        //CHECK_OGL_ERR("Mesh glBindVertexArray")
        
        glDrawElements(
            GL_TRIANGLES, 
            static_cast<GLsizei>(m_numIndices), 
            GL_UNSIGNED_INT, 
            0
            );
        //CHECK_OGL_ERR("Mesh glDrawElements")
        
        glBindVertexArray(0);
        //CHECK_OGL_ERR("Mesh glBindVertexArray")
    }
}

bool Mesh::isValid() const
{
    return m_VAO != 0;
}

void Mesh::setupBuffers(const MeshDesc& desc)
{
    glGenVertexArrays(1, &m_VAO);
    CHECK_OGL_ERR("Mesh glGenVertexArrays")
    glGenBuffers(1, &m_VBO);
    CHECK_OGL_ERR("Mesh glGenBuffers VBO")
    glGenBuffers(1, &m_EBO);
    CHECK_OGL_ERR("Mesh glGenBuffers EBO")
	
    glBindVertexArray(m_VAO);
    CHECK_OGL_ERR("Mesh glBindVertexArray")
	
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    CHECK_OGL_ERR("Mesh glBindBuffer GL_ARRAY_BUFFER")
    glBufferData(
        GL_ARRAY_BUFFER, 
        static_cast<GLsizei>(desc.m_vertices.size()), 
        desc.m_vertices.data(), 
        GL_STATIC_DRAW
        );
    CHECK_OGL_ERR("Mesh glBufferData GL_ARRAY_BUFFER")
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    CHECK_OGL_ERR("Mesh glVertexAttribPointer")
    glEnableVertexAttribArray(0);
    CHECK_OGL_ERR("Mesh glEnableVertexAttribArray")

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    CHECK_OGL_ERR("Mesh glBindBuffer GL_ELEMENT_ARRAY_BUFFER")
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,         
        static_cast<GLsizei>(desc.m_indices.size()), 
        desc.m_indices.data(), 
        GL_STATIC_DRAW
        );
    CHECK_OGL_ERR("Mesh glBufferData GL_ELEMENT_ARRAY_BUFFER")
	
    glBindVertexArray(0);
    CHECK_OGL_ERR("Mesh glBindVertexArray")
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_OGL_ERR("Mesh glBindBuffer GL_ARRAY_BUFFER")
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CHECK_OGL_ERR("Mesh glBindBuffer GL_ELEMENT_ARRAY_BUFFER")
}

void Mesh::destroyMesh()
{
    glDeleteVertexArrays(1, &m_VAO);
    CHECK_OGL_ERR("Mesh glDeleteVertexArrays")
    glDeleteBuffers(1, &m_VBO);
    CHECK_OGL_ERR("Mesh glDeleteBuffers VBO")
    glDeleteBuffers(1, &m_EBO);
    CHECK_OGL_ERR("Mesh glDeleteBuffers EBO")
    
    m_VAO = m_VBO = m_EBO = 0;
}
