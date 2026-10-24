#include "ResourceManager/Resources/MaterialResource.h"

#include <fstream>

#include "json.h"
#include "Rendering/Material/MaterialParameters/FloatParameter.h"
#include "Rendering/Material/MaterialParameters/Matrix4Parameter.h"
#include "Rendering/Material/MaterialParameters/TextureParameter.h"
#include "Rendering/Material/MaterialParameters/Vector3Parameter.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"


MaterialResource::~MaterialResource()
{
    MaterialResource::Free();
}

void MaterialResource::Reload()
{
    Free();

    const std::string fullPath = GetFullFilePath();
    std::ifstream materialFile(fullPath);
    if (!materialFile.is_open())
    {
        LOG_ERROR("Failed to open material file: {}", fullPath);
        return;
    }

    nlohmann::json materialAsset;
    try
    {
        materialFile >> materialAsset;
    }
    catch (const nlohmann::json::exception& exception)
    {
        LOG_ERROR("Failed to parse material file {}: {}", fullPath, exception.what());
        return;
    }

    if (!materialAsset.contains("shaderProgram") || !materialAsset["shaderProgram"].is_string())
    {
        LOG_ERROR("Invalid material file {}: shaderProgram is missing", fullPath);
        return;
    }
    ResourceManager& resourceManager = ResourceManager::Get();
    m_shaderProgramResource = resourceManager.LoadResource<ShaderProgramResource>(materialAsset["shaderProgram"]);

    ShaderProgramResource* shaderProgramResource = m_shaderProgramResource.Get();
    ShaderProgram* shaderProgram = shaderProgramResource ? shaderProgramResource->GetShaderProgram() : nullptr;
    
    if (!m_shaderProgramResource.IsValid() || shaderProgram == nullptr || !shaderProgram->IsValid())
    {
        LOG_ERROR("Failed to load shader program for material: {}", fullPath);
        Free();
        return;
    }

    m_material.emplace();
    m_material->SetShaderProgram(shaderProgram);

    if (materialAsset.contains("parameters") && !materialAsset["parameters"].is_array())
    {
        LOG_ERROR("Invalid material file {}: parameters must be an array", fullPath);
        Free();
        return;
    }

    shaderProgram->Activate();
    
    bool parametersValid = true;
    if (materialAsset.contains("parameters"))
    {
        for (const auto& parameter : materialAsset["parameters"])
        {
            if (!AddParameter(shaderProgram, parameter))
            {
                parametersValid = false;
                break;
            }
        }
    }
    
    shaderProgram->Deactivate();

    if (!parametersValid)
    {
        Free();
    }
}

void MaterialResource::Free()
{
    m_material.reset();
    m_shaderProgramResource = ResourceHandle<ShaderProgramResource>();
}

const Material* MaterialResource::GetMaterial() const
{
    return m_material ? &m_material.value() : nullptr;
}

Material* MaterialResource::GetMaterial()
{
    return m_material ? &m_material.value() : nullptr;
}

bool MaterialResource::IsValid() const
{
    return m_material && m_material->isValid();
}

namespace 
{
    template<typename TParam>
    bool PushParameter(Material& material, const nlohmann::json& json)
    {
        TParam parameter;
        if (!parameter.Deserialize(json))
        {
            return false;
        }
        material.PushParameter(parameter.GetName(), std::move(parameter));
        return true;
    };
}

bool MaterialResource::AddParameter(const ShaderProgram* shaderProgram, const nlohmann::json& json)
{
    if (shaderProgram == nullptr || !shaderProgram->IsValid())
    {
        LOG_ERROR("Shader program is invalid");
        return false;
    }

    if (!json.is_object() || !json.contains("type") || !json["type"].is_string())
    {
        LOG_ERROR("Invalid material parameter definition");
        return false;
    }

    const std::string type = json["type"];
    if (type == "FloatParameter")
    {
        return PushParameter<FloatParameter>(m_material.value(), json);
    }
    if (type == "Vector3Parameter")
    {
        return PushParameter<Vector3Parameter>(m_material.value(), json);
    }
    if (type == "Matrix4Parameter")
    {
        return PushParameter<Matrix4Parameter>(m_material.value(), json);
    }
    if (type == "TextureParameter")
    {
        return PushParameter<TextureParameter>(m_material.value(), json);
    }

    LOG_ERROR("Unsupported material parameter type: {}", type);
    return false;
}
