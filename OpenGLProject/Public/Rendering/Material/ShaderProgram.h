#pragma once

#include <unordered_map>
#include <vector>

#include "glad/glad.h"
#include "glm/fwd.hpp"

#include "ResourceManager/Resources/ShaderResource.h"

struct ShaderProgramDesc
{
    std::vector<const ShaderResource*> m_shaders;
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
    
    void Setup(const ShaderProgramDesc& desc);
    void Activate() const;
    void Deactivate() const;
    
    bool IsValid() const;
    
    GLuint GetShaderProgramID() const;
    
    void SetUniformInt(const std::string& name, int value);
    
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformFloat2(const std::string& name, const glm::vec2& value);
    void SetUniformFloat3(const std::string& name, const glm::vec3& value);
    void SetUniformFloat4(const std::string& name, const glm::vec4& value);
    
    void SetUniformMat3(const std::string& name, const glm::mat3& value);
    void SetUniformMat4(const std::string& name, const glm::mat4& value);
    
protected:
    
    GLint GetUniformLocation(const std::string& name) const;
    void CreateShaderProgram(const ShaderProgramDesc& desc);
    void DestroyShaderProgram();
    
private:
    
    GLuint m_shaderProgramID = 0;
    mutable std::unordered_map<std::string, GLint> m_uniforms;
    
};
