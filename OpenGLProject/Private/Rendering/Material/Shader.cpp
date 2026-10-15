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
    createShader(desc);
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
    destroyShader();
}

void Shader::setup(const ShaderDesc& desc)
{
    m_shaderType = desc.m_shaderType;
    
    destroyShader();
    createShader(desc);
}

bool Shader::isValid() const
{
    return m_shaderID != 0;
}

ShaderType Shader::getShaderType() const
{
    return m_shaderType;
}

GLenum Shader::getGLShaderType() const
{
    return g_enumToGLenum.at(m_shaderType);
}

GLuint Shader::getShaderID() const
{
    return m_shaderID;
}

void Shader::createShader(const ShaderDesc& desc)
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

    CHECK_OGL_ERR(m_shaderID = glCreateShader(getGLShaderType()))
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

void Shader::destroyShader()
{
    CHECK_OGL_ERR(glDeleteShader(m_shaderID))
    m_shaderID = 0;
}
