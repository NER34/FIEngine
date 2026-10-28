#include "ResourceManager/Resources/MaterialResource.h"

#include <fstream>
#include <ranges>

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
    m_shaderProgram = resourceManager.LoadResource<ShaderProgramResource>(materialAsset["shaderProgram"]);

    if (!IsValid())
    {
        LOG_ERROR("Failed to load shader program for material: {}", fullPath);
        Free();
        return;
    }

    if (materialAsset.contains("parameters") && !materialAsset["parameters"].is_array())
    {
        LOG_ERROR("Invalid material file {}: parameters must be an array", fullPath);
        Free();
        return;
    }

    m_shaderProgram->Activate();

    bool parametersValid = true;
    if (materialAsset.contains("parameters"))
    {
        for (const auto& parameter : materialAsset["parameters"])
        {
            if (!AddParameter(parameter))
            {
                parametersValid = false;
                break;
            }
        }
    }

    m_shaderProgram->Deactivate();

    if (!parametersValid)
    {
        Free();
    }
}

void MaterialResource::Free()
{
    m_parameters.ClearParameters();
    m_shaderProgram.Reset();
}

void MaterialResource::Activate() const
{
    if (IsValid())
    {
        m_shaderProgram->Activate();

        for (const auto& parameter : m_parameters.GetParameters() | std::views::values)
        {
            parameter->ApplyParameter(m_shaderProgram);
        }
    }
    else
    {
        LOG_ERROR("Failed activate material: shader program is invalid");
    }
}

void MaterialResource::Deactivate() const
{
    if (m_shaderProgram.IsValid())
    {
        m_shaderProgram->Deactivate();
    }
}

bool MaterialResource::IsValid() const
{
    return m_shaderProgram.IsValid() && m_shaderProgram->IsValid();
}

const ResourceHandle<ShaderProgramResource>& MaterialResource::GetShaderProgram() const
{
    return m_shaderProgram;
}

GLuint MaterialResource::GetShaderProgramID() const
{
    return m_shaderProgram.IsValid() ? m_shaderProgram->GetShaderProgramID() : 0;
}

namespace
{
    template<typename TParam>
    bool DeserializeParameter(MaterialResource& material, const nlohmann::json& json)
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

bool MaterialResource::AddParameter(const nlohmann::json& json)
{
    if (!IsValid())
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
        return DeserializeParameter<FloatParameter>(*this, json);
    }
    if (type == "Vector3Parameter")
    {
        return DeserializeParameter<Vector3Parameter>(*this, json);
    }
    if (type == "Matrix4Parameter")
    {
        return DeserializeParameter<Matrix4Parameter>(*this, json);
    }
    if (type == "TextureParameter")
    {
        return DeserializeParameter<TextureParameter>(*this, json);
    }

    LOG_ERROR("Unsupported material parameter type: {}", type);
    return false;
}
