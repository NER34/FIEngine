#pragma once

#include <string>

#include "glad/glad.h"

struct ShaderDesc
{
    GLenum m_shaderType;
    std::string m_shaderFilePath;
};

class Shader
{
public:
    
    Shader() = default;
    Shader(const ShaderDesc& desc);
    
    Shader(const Shader& other) = default;
    Shader& operator=(const Shader& other) = default;
    
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    
    ~Shader();
    
    void setup(const ShaderDesc& desc);
    
    bool isValid() const;
    
    GLenum getShaderType() const;
    GLuint getShaderID() const;
    
protected:
    
    void createShader(const std::string& shaderFilePath);
    void destroyShader();
    
private:
    
    GLenum m_shaderType = GL_VERTEX_SHADER; 
    GLuint m_shaderID = 0;
    
};
