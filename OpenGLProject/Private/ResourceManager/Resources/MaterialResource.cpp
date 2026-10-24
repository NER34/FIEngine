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

    if (materialAsset.contains("textures") && !materialAsset["textures"].is_array())
    {
        LOG_ERROR("Invalid material file {}: textures must be an array", fullPath);
        return;
    }

    ResourceManager& resourceManager = ResourceManager::Get();
    m_shaderProgramResource = resourceManager.LoadResource<ShaderProgramResource>(
        materialAsset["shaderProgram"].get<std::string>()
        );

    ShaderProgramResource* shaderProgramResource = m_shaderProgramResource.Get();
    ShaderProgram* shaderProgram = shaderProgramResource
        ? shaderProgramResource->GetShaderProgram()
        : nullptr;
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
    m_textureResources.clear();
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

bool MaterialResource::AddParameter(const ShaderProgram* shaderProgram, const nlohmann::json& json)
{
    if (shaderProgram == nullptr || !shaderProgram->IsValid() ||
        !json.is_object() || !json.contains("type") || !json["type"].is_string())
    {
        LOG_ERROR("Invalid material parameter definition");
        return false;
    }

    const std::string type = json["type"];
    const GLuint shaderProgramID = shaderProgram->GetShaderProgramID();

    if (type == "FloatParameter")
    {
        if (!json.contains("name") || !json["name"].is_string())
        {
            LOG_ERROR("Material parameter is missing its name");
            return false;
        }
        const std::string name = json["name"];

        if (!json.contains("value") || !json["value"].is_number())
        {
            LOG_ERROR("Float material parameter {} has an invalid value", name);
            return false;
        }

        m_material->PushParameter(name, FloatParameter(name, json["value"]));
        return true;
    }

    if (type == "Vector3Parameter")
    {
        if (!json.contains("name") || !json["name"].is_string())
        {
            LOG_ERROR("Material parameter is missing its name");
            return false;
        }
        const std::string name = json["name"];

        if (!json.contains("value") || !json["value"].is_array() || json["value"].size() != 3)
        {
            LOG_ERROR("Vector3 material parameter {} must contain three values", name);
            return false;
        }

        glm::vec3 value;
        for (std::size_t index = 0; index < 3; ++index)
        {
            if (!json["value"][index].is_number())
            {
                LOG_ERROR("Vector3 material parameter {} has an invalid value", name);
                return false;
            }
            value[index] = json["value"][index];
        }

        m_material->PushParameter(name, Vector3Parameter(name, value));
        return true;
    }

    if (type == "Matrix4Parameter")
    {
        if (!json.contains("name") || !json["name"].is_string())
        {
            LOG_ERROR("Material parameter is missing its name");
            return false;
        }
        const std::string name = json["name"];

        if (!json.contains("value") || !json["value"].is_array() || json["value"].size() != 16)
        {
            LOG_ERROR("Matrix4 material parameter {} must contain sixteen values", name);
            return false;
        }

        glm::mat4 value(1.0f);
        for (std::size_t index = 0; index < 16; ++index)
        {
            if (!json["value"][index].is_number())
            {
                LOG_ERROR("Matrix4 material parameter {} has an invalid value", name);
                return false;
            }
            value[index % 4][index / 4] = json["value"][index].get<float>();
        }

        m_material->PushParameter(name, Matrix4Parameter(name, value));
        return true;
    }

    if (type == "TextureParameter")
    {
        if (!json.contains("name") || !json["name"].is_string())
        {
            LOG_ERROR("Material parameter is missing its name");
            return false;
        }
        const std::string name = json["name"];

        if (!json.contains("slot") || !json["slot"].is_number())
        {
            LOG_ERROR("Texture material parameter {} has an invalid slot", name);
            return false;
        }

        if (!json.contains("path") || !json["path"].is_string())
        {
            LOG_ERROR("Texture material parameter {} has invalid path", name);
            return false;
        }

        const GLuint textureSlot = json["slot"];
        auto textureResource = ResourceManager::Get().LoadResource<TextureResource>(json["path"]);

        Texture* texture = textureResource.IsValid() ? textureResource.Get()->GetTexture() : nullptr;
        if (texture == nullptr || !texture->isValid())
        {
            LOG_ERROR(
                "Failed to load texture for material parameter {}: {}", 
                name, json["path"].get<std::string>()
                );
            return false;
        }

        m_textureResources.push_back(textureResource);
        m_material->PushParameter(name, TextureParameter(name, texture, textureSlot));
        return true;
    }

    LOG_ERROR("Unsupported material parameter type: {}", type);
    return false;
}
