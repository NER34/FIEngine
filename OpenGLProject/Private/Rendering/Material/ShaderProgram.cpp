#include "Rendering/Material/ShaderProgram.h"

#include "Rendering/Validation.h"
#include "Utilities/Logger.h"

ShaderProgram::ShaderProgram(const ShaderProgramDesc& desc)
{
    createShaderProgram(desc);
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
        destroyShaderProgram();
        
        m_shaderProgramID = other.m_shaderProgramID;
        other.m_shaderProgramID = 0;
    }
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    destroyShaderProgram();
}

void ShaderProgram::setup(const ShaderProgramDesc& desc)
{
    destroyShaderProgram();
    createShaderProgram(desc);
}

void ShaderProgram::activate() const
{
    if (isValid())
    {
        CHECK_OGL_ERR(glUseProgram(m_shaderProgramID))
    }
    else
    {
        LOG_ERROR("Failed activate shader program: m_shaderProgramID is invalid");
    }
}

void ShaderProgram::deactivate() const
{
    CHECK_OGL_ERR(glUseProgram(0))
}

bool ShaderProgram::isValid() const
{
    return m_shaderProgramID != 0;
}

GLuint ShaderProgram::getShaderProgramID() const
{
    return m_shaderProgramID;
}

void ShaderProgram::createShaderProgram(const ShaderProgramDesc& desc)
{
    CHECK_OGL_ERR(m_shaderProgramID = glCreateProgram())
    
    for (const Shader* shader : desc.m_shaders)
    {
        if (shader == nullptr || !shader->isValid())
        {
            LOG_ERROR("Failed to attach shader: shader is invalid");
            continue;
        }
        
        CHECK_OGL_ERR(glAttachShader(m_shaderProgramID, shader->getShaderID()))
    }
    
    CHECK_OGL_ERR(glLinkProgram(m_shaderProgramID))
    
    GLint linkStatus = 0;
    CHECK_OGL_ERR(glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &linkStatus))
    
    if (linkStatus == GL_FALSE)
    {
        GLchar infoLog[1024] = {};
        CHECK_OGL_ERR(glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog))
        LOG_ERROR("Failed to link shader program: {}", infoLog);
        destroyShaderProgram();
    }
}

void ShaderProgram::destroyShaderProgram()
{
    if (isValid())
    {
        CHECK_OGL_ERR(glDeleteProgram(m_shaderProgramID))
        m_shaderProgramID = 0;
    }
}
