#pragma once

#include <vector>

#include "glad/glad.h"

#include "Rendering/Material/Shader.h"

struct ShaderProgramDesc
{
    std::vector<const Shader*> m_shaders;
};

class ShaderProgram
{
public:
    
    ShaderProgram() = default;
    ShaderProgram(const ShaderProgramDesc& desc);
    
    ShaderProgram(const ShaderProgram& other) = default;
    ShaderProgram& operator=(const ShaderProgram& other) = default;
    
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;
    
    ~ShaderProgram();
    
    void setup(const ShaderProgramDesc& desc);
    void activate() const;
    void deactivate() const;
    
    bool isValid() const;
    
    GLuint getShaderProgramID() const;
    
protected:
    
    void createShaderProgram(const ShaderProgramDesc& desc);
    void destroyShaderProgram();
    
private:
    
    GLuint m_shaderProgramID = 0;
    
};
