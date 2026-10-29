#include "ResourceManager/Resources/ShaderProgramResource.h"

#include "glm/gtc/type_ptr.hpp"

#include "AssetManager/AssetManager.h"
#include "Rendering/Validation.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

ShaderProgramResource::~ShaderProgramResource()
{
    ShaderProgramResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, ShaderProgramResource)
{
    resource.Free();

    if (!data.contains("shaders") || !data["shaders"].is_array() || data["shaders"].empty())
    {
        LOG_ERROR("Invalid shader program asset file: shaders are missing");
        return false;
    }

    ShaderProgramDesc shaderProgramDesc;
    for (const auto& shaderAssetPath : data["shaders"])
    {
        if (!shaderAssetPath.is_string())
        {
            LOG_ERROR("Invalid shader asset path");
            continue;
        }
        
        ResourceHandle<ShaderResource> shaderHandle;
        if (!AssetManager::Get().Load<ShaderResource>(shaderAssetPath, shaderHandle) || !shaderHandle->IsValid())
        {
            LOG_ERROR("Failed to load shader for program: invalid shader resource");
            continue;
        }
        shaderProgramDesc.m_shaders.push_back(shaderHandle);
    }

    return resource.CreateShaderProgram(shaderProgramDesc);
}

void ShaderProgramResource::Free()
{
    DestroyShaderProgram();
    m_uniforms.clear();
    m_shaderResources.clear();
}

void ShaderProgramResource::Activate() const
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glUseProgram(m_shaderProgramID))
    }
    else
    {
        LOG_ERROR("Failed activate shader program: m_shaderProgramID is invalid");
    }
}

void ShaderProgramResource::Deactivate() const
{
    CHECK_OGL_ERR(glUseProgram(0))
}

bool ShaderProgramResource::IsValid() const
{
    return m_shaderProgramID != 0;
}

GLuint ShaderProgramResource::GetShaderProgramID() const
{
    return m_shaderProgramID;
}

namespace
{
    class ActivateShaderProgram_Scoped
    {
    public:
        ActivateShaderProgram_Scoped(GLint shaderProgramID)
        {
            CHECK_OGL_ERR(glGetIntegerv(GL_CURRENT_PROGRAM, &m_prevShaderProgramID))
            if (shaderProgramID != m_prevShaderProgramID)
            {
                CHECK_OGL_ERR(glUseProgram(shaderProgramID))
            }
            else
            {
                m_prevShaderProgramID = -1;
            }
        }
        ~ActivateShaderProgram_Scoped()
        {
            if (m_prevShaderProgramID != -1)
            {
                CHECK_OGL_ERR(glUseProgram(m_prevShaderProgramID))
            }
        }
    private:
        GLint m_prevShaderProgramID = -1;
    };

#define USE_SHADER_PROGRAM_SCOPED(shaderProgramID) \
    ActivateShaderProgram_Scoped scoped_##shaderProgramID(shaderProgramID)
}

void ShaderProgramResource::SetUniformInt(const std::string& name, int value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform1i(GetUniformLocation(name), value))
}

void ShaderProgramResource::SetUniformFloat(const std::string& name, float value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform1f(GetUniformLocation(name), value))
}

void ShaderProgramResource::SetUniformFloat2(const std::string& name, const glm::vec2& value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform2f(GetUniformLocation(name), value.x, value.y))
}

void ShaderProgramResource::SetUniformFloat3(const std::string& name, const glm::vec3& value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z))
}

void ShaderProgramResource::SetUniformFloat4(const std::string& name, const glm::vec4& value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w))
}

void ShaderProgramResource::SetUniformMat3(const std::string& name, const glm::mat3& value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(value)))
}

void ShaderProgramResource::SetUniformMat4(const std::string& name, const glm::mat4& value) const
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(value)))
}

GLint ShaderProgramResource::GetUniformLocation(const std::string& name) const
{
    if (auto it = m_uniforms.find(name); it != m_uniforms.end())
    {
        return it->second;
    }
    GLint location;
    CHECK_OGL_ERR(location = glGetUniformLocation(m_shaderProgramID, name.c_str()))
    m_uniforms[name] = location;
    return location;
}

bool ShaderProgramResource::CreateShaderProgram(const ShaderProgramDesc& desc)
{
    Free();

    if (desc.m_shaders.empty())
    {
        LOG_ERROR("Failed to create shader program: no shaders provided");
        return false;
    }

    CHECK_OGL_ERR(m_shaderProgramID = glCreateProgram())

    for (const ResourceHandle<ShaderResource>& shader : desc.m_shaders)
    {
        if (!shader.IsValid() || !shader->IsValid())
        {
            LOG_ERROR("Failed to attach shader: shader is invalid");
            continue;
        }

        CHECK_OGL_ERR(glAttachShader(m_shaderProgramID, shader->GetShaderID()))
        m_shaderResources.push_back(shader);
    }

    CHECK_OGL_ERR(glLinkProgram(m_shaderProgramID))

    GLint linkStatus = 0;
    CHECK_OGL_ERR(glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus))

    if (linkStatus == GL_FALSE)
    {
        GLchar infoLog[1024] = {};
        CHECK_OGL_ERR(glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog))
        LOG_ERROR("Failed to link shader program: {}", infoLog);
        Free();
        return false;
    }
    return true;
}

void ShaderProgramResource::DestroyShaderProgram()
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glDeleteProgram(m_shaderProgramID))
        m_shaderProgramID = 0;
    }
}

REGISTER_ASSET_FACTORY(ShaderProgramResource, ShaderProgramResource::Deserialize)
