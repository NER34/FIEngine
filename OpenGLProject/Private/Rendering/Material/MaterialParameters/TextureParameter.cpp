#include "Rendering/Material/MaterialParameters/TextureParameter.h"

#include "AssetManager/AssetManager.h"
#include "ResourceManager/Resources/ShaderProgramResource.h"


void TextureParameter::ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const
{
    if (m_textureHandle->IsValid())
    {
        shaderProgram->SetUniformInt(m_name, m_textureSlot);
        m_textureHandle->Activate(GL_TEXTURE0 + m_textureSlot);
    }
}

DEFINE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, TextureParameter)
{
    if (!data.contains("slot") || !data["slot"].is_number())
    {
        LOG_ERROR("Texture material parameter {} has an invalid slot", parameter.GetName());
        return false;
    }
    parameter.m_textureSlot = data["slot"];

    if (!data.contains("path") || !data["path"].is_string())
    {
        LOG_ERROR("Texture material parameter {} has invalid path", parameter.GetName());
        return false;
    }
    if (!AssetManager::Get().Load<TextureResource>(data["path"], parameter.m_textureHandle))
    {
        LOG_ERROR("Texture material parameter {} failed to load texture", parameter.GetName());
        return false;
    }
    
    return true;
}

REGISTER_MATERIAL_PARAM_FACTORY(TextureParameter, TextureParameter::Deserialize)
