#include "ResourceManager/Resources/ShaderResource.h"

#include <cassert>
#include <fstream>
#include <unordered_map>

#include "Rendering/Validation.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

const std::unordered_map<ShaderType, GLenum> g_enumToGLenum = 
{
    { ShaderType::Vertex, GL_VERTEX_SHADER },
    { ShaderType::Fragment, GL_FRAGMENT_SHADER },
    { ShaderType::Geometry, GL_GEOMETRY_SHADER },
    { ShaderType::Compute, GL_COMPUTE_SHADER }
};

ShaderResource::~ShaderResource()
{
    ShaderResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, ShaderResource)
{
    resource.Free();

    if (!data.contains("shaderType") || !data["shaderType"].is_number_integer())
    {
        LOG_ERROR("Invalid shader asset file {}: shaderType is missing", resource.GetFilePath());
        return false;
    }
    const ShaderType shaderType = data["shaderType"].get<ShaderType>();
    if (!g_enumToGLenum.contains(shaderType))
    {
        LOG_ERROR("Invalid shader asset file {}: unknown shaderType {}", resource.GetFilePath(), data["shaderType"].get<int>());
        return false;
    }

    if (!data.contains("path") || !data["path"].is_string())
    {
        LOG_ERROR("Invalid shader asset file {}: path is missing", resource.GetFilePath());
        return false;
    }
    
    const std::filesystem::path shaderPath = GetResourcesPath() / data["path"].get<std::string>();
    std::ifstream shaderFile(shaderPath);

    if (!shaderFile.is_open())
    {
        LOG_ERROR("Failed to open shader file: {}", shaderPath.string());
        return false;
    }

    std::string shaderData = std::string(
        std::istreambuf_iterator<char>(shaderFile),
        std::istreambuf_iterator<char>()
        );
    
    ShaderDesc shaderDesc;
    shaderDesc.m_shaderType = shaderType;
    shaderDesc.m_shaderDataPtr = shaderData.c_str();
    shaderDesc.m_shaderDataSize = static_cast<int>(shaderData.size());
#if DEBUG_ENABLED
    shaderDesc.m_shaderFilePath = shaderPath.string();
#endif

    return resource.CreateShader(shaderDesc);
}

void ShaderResource::Free()
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glDeleteShader(m_shaderID))
        m_shaderID = 0;
    }
    m_shaderType = ShaderType::Invalid;
}

bool ShaderResource::IsValid() const
{
    return m_shaderID != 0;
}

ShaderType ShaderResource::GetShaderType() const
{
    return m_shaderType;
}

GLenum ShaderResource::GetGLShaderType() const
{
    return g_enumToGLenum.at(m_shaderType);
}

GLuint ShaderResource::GetShaderID() const
{
    return m_shaderID;
}

bool ShaderResource::CreateShader(const ShaderDesc& desc)
{
    Free();
    
    m_shaderType = desc.m_shaderType;
    
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
        Free();
        return false;
    }
    return true;
}

REGISTER_ASSET_FACTORY(ShaderResource, ShaderResource::Deserialize)
