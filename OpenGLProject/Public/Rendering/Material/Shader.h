#pragma once

#include <string>

#include "glad/glad.h"

enum class ShaderType : char
{
    Invalid = -1,
    
    Vertex = 0,
    Fragment,
    Geometry,
    Compute
};

struct ShaderDesc
{
#if DEBUG_ENABLED
    std::string m_shaderFilePath;
#endif
    
    int m_shaderDataSize = 0;
    ShaderType m_shaderType = ShaderType::Vertex;
    const GLchar* m_shaderDataPtr = nullptr;
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
    
    ShaderType getShaderType() const;
    GLenum getGLShaderType() const;
    GLuint getShaderID() const;
    
protected:
    
    void createShader(const ShaderDesc& desc);
    void destroyShader();
    
private:
    
    ShaderType m_shaderType = ShaderType::Invalid; 
    GLuint m_shaderID = 0;
    
};
