#include "ResourceManager/Resources/ShaderProgramResource.h"

#include <fstream>

#include "json.h"

#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

ShaderProgramResource::~ShaderProgramResource()
{
    ShaderProgramResource::Free();
}

void ShaderProgramResource::Reload()
{
    Free();
    
    const std::string fullPath = GetFullFilePath();
    std::ifstream shaderProgramFile(fullPath);
    if (!shaderProgramFile.is_open())
    {
        LOG_ERROR("Failed to open shader program file: {}", fullPath);
        return;
    }
    
    nlohmann::json shaderProgramAsset;
    shaderProgramFile >> shaderProgramAsset;
    if (!shaderProgramAsset.contains("shaders") || !shaderProgramAsset["shaders"].is_array())
    {
        LOG_ERROR("Invalid shader program asset file: {}", fullPath);
        return;
    }
    
    for (const auto& shaderAssetPath : shaderProgramAsset["shaders"])
    {
        if (!shaderAssetPath.is_string())
        {
            LOG_ERROR("Invalid shader asset path in shader program file: {}", fullPath);
            continue;
        }
        m_shaderResources.push_back(ResourceManager::Get().LoadResource<ShaderResource>(shaderAssetPath));
    }
    if (m_shaderResources.empty())
    {
        LOG_ERROR("Failed to create shader program {}: no shaders declared", GetFilePath());
        return;
    }
    
    ShaderProgramDesc shaderProgramDesc;
    for (const ResourceHandle<ShaderResource>& shaderResourceHandle : m_shaderResources)
    {
        if (!shaderResourceHandle.IsValid() || !shaderResourceHandle.Get()->IsValid())
        {
            LOG_ERROR("Failed to load shader for program {}: invalid shader resource", GetFilePath());
            continue;
        }
        shaderProgramDesc.m_shaders.push_back(shaderResourceHandle.Get());
    }
    
    m_shaderProgram.emplace(shaderProgramDesc);
}

void ShaderProgramResource::Free()
{
    m_shaderProgram.reset();
    m_shaderResources.clear();
}

ShaderProgram* ShaderProgramResource::GetShaderProgram()
{
    return m_shaderProgram ? &m_shaderProgram.value() : nullptr;
}
