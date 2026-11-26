#include "ResourceManager/Resources/MaterialResource.h"

#include <ranges>

#include "json.h"
#include "AssetManager/AssetManager.h"
#include "easy/profiler.h"
#include "MaterialParameters/MaterialParamFactory.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"
#include "Utilities/ParsingHelpers.h"


MaterialResource::~MaterialResource()
{
    MaterialResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, MaterialResource)
{
    resource.Free();

    if (!HasStringField(data, "shaderProgram"))
    {
        LOG_ERROR("Invalid material file: shaderProgram is missing");
        return false;
    }
    AssetManager::Get().Load<ShaderProgramResource>(data["shaderProgram"], resource.m_shaderProgram);

    if (!resource.IsValid())
    {
        LOG_ERROR("Failed to load shader program for material");
        resource.Free();
        return false;
    }

    if (!IsOptionalArrayField(data, "parameters"))
    {
        LOG_ERROR("Invalid material file: parameters must be an array");
        resource.Free();
        return false;
    }

    resource.m_shaderProgram->Activate();

    bool parametersValid = true;
    if (data.contains("parameters"))
    {
        for (const auto& parameter : data["parameters"])
        {
            if (!resource.AddParameter(parameter))
            {
                parametersValid = false;
                break;
            }
        }
    }

    resource.m_shaderProgram->Deactivate();

    if (!parametersValid)
    {
        resource.Free();
        return false;
    }
    return true;
}

void MaterialResource::Free()
{
    m_parameters.ClearParameters();
    m_shaderProgram.Reset();
}

void MaterialResource::Activate() const
{
    EASY_FUNCTION()
    
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

void MaterialResource::SetShaderProgram(const ResourceHandle<ShaderProgramResource>& shaderProgram)
{
    m_shaderProgram = shaderProgram;
}

const ResourceHandle<ShaderProgramResource>& MaterialResource::GetShaderProgram() const
{
    return m_shaderProgram;
}

GLuint MaterialResource::GetShaderProgramID() const
{
    return m_shaderProgram.IsValid() ? m_shaderProgram->GetShaderProgramID() : 0;
}

bool MaterialResource::AddParameter(const nlohmann::json& json)
{
    if (!IsValid())
    {
        LOG_ERROR("Shader program is invalid");
        return false;
    }

    if (!json.is_object() || !HasStringField(json, "type"))
    {
        LOG_ERROR("Invalid material parameter definition");
        return false;
    }

    if (!HasStringField(json, "name"))
    {
        LOG_ERROR("Material parameter is missing its name");
        return false;
    }

    const std::string type = json["type"];
    IMaterialParamFactory* paramFactory = MaterialParamRegistry::Get().GetObject(type);
    if (!paramFactory)
    {
        LOG_ERROR("Unsupported material parameter type: {}", type);
        return false;
    }

    return paramFactory->CreateFromAsset(*this, json["name"], json);
}

REGISTER_ASSET_FACTORY(MaterialResource, MaterialResource::Deserialize)
