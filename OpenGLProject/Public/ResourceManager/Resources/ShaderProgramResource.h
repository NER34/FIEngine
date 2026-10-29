#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "glad/glad.h"
#include "glm/fwd.hpp"

#include "AssetManager/AssetFactory.h"
#include "ResourceManager/ResourceBase.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ShaderResource.h"

struct ShaderProgramDesc
{
    std::vector<ResourceHandle<ShaderResource>> m_shaders;
};

class ShaderProgramResource : public ResourceBase
{
public:

    ShaderProgramResource() = default;
    ~ShaderProgramResource() override;

    ShaderProgramResource(const ShaderProgramResource& other) = delete;
    ShaderProgramResource& operator=(const ShaderProgramResource& other) = delete;

    void Free() override;

    void Activate() const;
    void Deactivate() const;

    bool IsValid() const;

    GLuint GetShaderProgramID() const;

    void SetUniformInt(const std::string& name, int value) const;

    void SetUniformFloat(const std::string& name, float value) const;
    void SetUniformFloat2(const std::string& name, const glm::vec2& value) const;
    void SetUniformFloat3(const std::string& name, const glm::vec3& value) const;
    void SetUniformFloat4(const std::string& name, const glm::vec4& value) const;

    void SetUniformMat3(const std::string& name, const glm::mat3& value) const;
    void SetUniformMat4(const std::string& name, const glm::mat4& value) const;

    bool CreateShaderProgram(const ShaderProgramDesc& desc);

    DECLARE_ASSET_DESERIALIZE_FUNC(Deserialize, ShaderProgramResource);

private:

    GLint GetUniformLocation(const std::string& name) const;
    void DestroyShaderProgram();

    GLuint m_shaderProgramID = 0;
    mutable std::unordered_map<std::string, GLint> m_uniforms;

    std::vector<ResourceHandle<ShaderResource>> m_shaderResources;

};
