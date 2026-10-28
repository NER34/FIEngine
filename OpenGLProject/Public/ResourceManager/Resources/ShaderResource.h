#pragma once

#include <string>

#include "glad/glad.h"

#include "ResourceManager/ResourceBase.h"

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

class ShaderResource : public ResourceBase
{
public:
    
    ShaderResource() = default;
    ~ShaderResource() override;
    
    ShaderResource(const ShaderResource& other) = delete;
    ShaderResource& operator=(const ShaderResource& other) = delete;
    
    void Reload() override;
    void Free() override;
    
    bool IsValid() const;
    
    ShaderType GetShaderType() const;
    GLenum GetGLShaderType() const;
    GLuint GetShaderID() const;
    
private:
    
    void CreateShader(const ShaderDesc& desc);
    
    ShaderType m_shaderType = ShaderType::Invalid; 
    GLuint m_shaderID = 0;
    
};
