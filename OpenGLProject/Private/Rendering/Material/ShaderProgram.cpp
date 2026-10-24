#include "Rendering/Material/ShaderProgram.h"

#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Validation.h"
#include "Utilities/Logger.h"

ShaderProgram::ShaderProgram(const ShaderProgramDesc& desc)
{
    CreateShaderProgram(desc);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
{
    m_shaderProgramID = other.m_shaderProgramID;
    other.m_shaderProgramID = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        DestroyShaderProgram();
        
        m_shaderProgramID = other.m_shaderProgramID;
        other.m_shaderProgramID = 0;
    }
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    DestroyShaderProgram();
}

void ShaderProgram::Setup(const ShaderProgramDesc& desc)
{
    DestroyShaderProgram();
    CreateShaderProgram(desc);
}

void ShaderProgram::Activate() const
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

void ShaderProgram::Deactivate() const
{
    CHECK_OGL_ERR(glUseProgram(0))
}

bool ShaderProgram::IsValid() const
{
    return m_shaderProgramID != 0;
}

GLuint ShaderProgram::GetShaderProgramID() const
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

void ShaderProgram::SetUniformInt(const std::string& name, int value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform1i(GetUniformLocation(name), value))
}

void ShaderProgram::SetUniformFloat(const std::string& name, float value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform1f(GetUniformLocation(name), value))
}

void ShaderProgram::SetUniformFloat2(const std::string& name, const glm::vec2& value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform2f(GetUniformLocation(name), value.x, value.y))
}

void ShaderProgram::SetUniformFloat3(const std::string& name, const glm::vec3& value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z))
}

void ShaderProgram::SetUniformFloat4(const std::string& name, const glm::vec4& value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w))
}

void ShaderProgram::SetUniformMat3(const std::string& name, const glm::mat3& value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(value)))
}

void ShaderProgram::SetUniformMat4(const std::string& name, const glm::mat4& value)
{
    USE_SHADER_PROGRAM_SCOPED(m_shaderProgramID);
    CHECK_OGL_ERR(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(value)))
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) const
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

void ShaderProgram::CreateShaderProgram(const ShaderProgramDesc& desc)
{
    CHECK_OGL_ERR(m_shaderProgramID = glCreateProgram())
    
    for (const Shader* shader : desc.m_shaders)
    {
        if (shader == nullptr || !shader->IsValid())
        {
            LOG_ERROR("Failed to attach shader: shader is invalid");
            continue;
        }
        
        CHECK_OGL_ERR(glAttachShader(m_shaderProgramID, shader->GetShaderID()))
    }
    
    CHECK_OGL_ERR(glLinkProgram(m_shaderProgramID))
    
    GLint linkStatus = 0;
    CHECK_OGL_ERR(glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus))
    
    if (linkStatus == GL_FALSE)
    {
        GLchar infoLog[1024] = {};
        CHECK_OGL_ERR(glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog))
        LOG_ERROR("Failed to link shader program: {}", infoLog);
        DestroyShaderProgram();
    }
}

void ShaderProgram::DestroyShaderProgram()
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glDeleteProgram(m_shaderProgramID))
        m_shaderProgramID = 0;
    }
}
