#include "ResourceManager/Resources/ShaderResource.h"

#include <cassert>
#include <fstream>

#include "json.h"

#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

ShaderResource::~ShaderResource()
{
    ShaderResource::Free();
}

void ShaderResource::Reload()
{
    Free();
    
    const auto assetPath = GetResourcesPath() / GetFilePath();
    std::ifstream assetFile(assetPath);

    if (!assetFile.is_open())
    {
        LOG_ERROR("Failed to open shader asset file: {}", assetPath.string());
        return;
    }
    
    nlohmann::json shaderAsset;
    assetFile >> shaderAsset;
    assetFile.close(); 
    
    ShaderType shaderType = shaderAsset.value("type", ShaderType::Invalid);
    const std::string shaderFilePath = shaderAsset.value("path", "");
    
    if (shaderType == ShaderType::Invalid || shaderFilePath.empty())
    {
        LOG_ERROR("Invalid shader asset file: {}", assetPath.string());
        return;
    }
    
    const std::filesystem::path shaderPath = GetResourcesPath() / shaderFilePath;
    std::ifstream shaderFile(shaderPath);

    if (!shaderFile.is_open())
    {
        LOG_ERROR("Failed to open shader file: {}", shaderPath.string());
        return;
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

    m_shader.emplace(shaderDesc);
}

void ShaderResource::Free()
{
    m_shader.reset();
}

const Shader* ShaderResource::GetShader() const
{
    return m_shader ? &m_shader.value() : nullptr;
}

bool ShaderResource::IsValid() const
{
    return m_shader && m_shader->IsValid();
}
