#include "Rendering/Material/Shader.h"

#include <fstream>
#include <unordered_map>

#include "Rendering/Validation.h"

const std::unordered_map<ShaderType, GLenum> g_enumToGLenum = 
{
    { ShaderType::Vertex, GL_VERTEX_SHADER },
    { ShaderType::Fragment, GL_FRAGMENT_SHADER },
    { ShaderType::Geometry, GL_GEOMETRY_SHADER },
    { ShaderType::Compute, GL_COMPUTE_SHADER }
};

Shader::Shader(const ShaderDesc& desc)
    : m_shaderType(desc.m_shaderType)
{
    CreateShader(desc);
}

Shader::Shader(Shader&& other) noexcept
{
    m_shaderType = other.m_shaderType;
    m_shaderID = other.m_shaderID;
    other.m_shaderID = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        m_shaderType = other.m_shaderType;
        m_shaderID = other.m_shaderID;
        other.m_shaderID = 0;
    }
    return *this;
}

Shader::~Shader()
{
    DestroyShader();
}

void Shader::Setup(const ShaderDesc& desc)
{
    m_shaderType = desc.m_shaderType;
    
    DestroyShader();
    CreateShader(desc);
}

bool Shader::IsValid() const
{
    return m_shaderID != 0;
}

ShaderType Shader::GetShaderType() const
{
    return m_shaderType;
}

GLenum Shader::GetGLShaderType() const
{
    return g_enumToGLenum.at(m_shaderType);
}

GLuint Shader::GetShaderID() const
{
    return m_shaderID;
}

void Shader::CreateShader(const ShaderDesc& desc)
{
    
    /*
    // Read shader file
    std::ifstream shaderFile(shaderFilePath);
    
    if (!shaderFile.is_open())
    {
        LOG_ERROR("Failed to open shader file: {}", shaderFilePath);
        return;
    }
	
    std::string shaderData = std::string(
        std::istreambuf_iterator<char>(shaderFile),
        std::istreambuf_iterator<char>()
        );
    
    const char* shaderDataPtr = shaderData.c_str();
    int shaderDataSize = static_cast<GLint>(shaderData.size());
    */

    CHECK_OGL_ERR(m_shaderID = glCreateShader(GetGLShaderType()))
    CHECK_OGL_ERR(glShaderSource(m_shaderID, 1, &desc.m_shaderDataPtr, &desc.m_shaderDataSize))
    CHECK_OGL_ERR(glCompileShader(m_shaderID))

    GLint compileStatus = 0;
    CHECK_OGL_ERR(glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compileStatus))

    if (compileStatus == GL_FALSE)
    {
        GLchar infoLog[1024] = {};
        CHECK_OGL_ERR(glGetShaderInfoLog(m_shaderID, sizeof(infoLog), nullptr, infoLog))
#if DEBUG_ENABLED
        LOG_ERROR("Failed to compile shader {}: {}", desc.m_shaderFilePath, infoLog);
#endif
    }
}

void Shader::DestroyShader()
{
    CHECK_OGL_ERR(glDeleteShader(m_shaderID))
    m_shaderID = 0;
}
