#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Material.h"

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
    
    const glm::vec3& getPosition() const;
    const glm::vec3& getRotation() const;
    const glm::vec3& getScale() const;
    const glm::mat4& getModelMatrix();
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);
    
    bool isValid() const;

protected:
    
    void setupBuffers(const MeshDesc& desc);
    void destroyMesh();
    
private:

    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;
    
    uint32_t m_numIndices = 0;
    
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    
    glm::mat4 m_modelMatrix = glm::mat4(1.0f);
    
};
