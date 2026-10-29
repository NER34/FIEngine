#include "ResourceManager/Resources/MaterialResource.h"

#include <ranges>

#include "json.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Material/MaterialParamFactory.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Logger.h"


MaterialResource::~MaterialResource()
{
    MaterialResource::Free();
}

DEFINE_ASSET_DESERIALIZE_FUNC(Deserialize, MaterialResource)
{
    resource.Free();

    if (!data.contains("shaderProgram") || !data["shaderProgram"].is_string())
    {
        LOG_ERROR("Invalid material file {}: shaderProgram is missing", resource.GetFilePath());
        return false;
    }
    AssetManager::Get().Load<ShaderProgramResource>(data["shaderProgram"], resource.m_shaderProgram);

    if (!resource.IsValid())
    {
        LOG_ERROR("Failed to load shader program for material: {}", resource.GetFilePath());
        resource.Free();
        return false;
    }

    if (data.contains("parameters") && !data["parameters"].is_array())
    {
        LOG_ERROR("Invalid material file {}: parameters must be an array", resource.GetFilePath());
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

    if (!json.is_object() || !json.contains("type") || !json["type"].is_string())
    {
        LOG_ERROR("Invalid material parameter definition");
        return false;
    }

    if (!json.contains("name") || !json["name"].is_string())
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
