#include "Rendering/Material/MaterialParameters/TextureParameter.h"

#include "Rendering/Material/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "ResourceManager/ResourceManager.h"


void TextureParameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    const TextureResource* textureResource = m_textureHandle.Get();
    if (auto* texture = textureResource->GetTexture())
    {
        shaderProgram->SetUniformInt(m_name, m_textureSlot);
        texture->activate(GL_TEXTURE0 + m_textureSlot);
    }
}

bool TextureParameter::Deserialize(const nlohmann::json& data)
{
    if (!IMaterialParameter::Deserialize(data))
    {
        return false;
    }

    if (!data.contains("slot") || !data["slot"].is_number())
    {
        LOG_ERROR("Texture material parameter {} has an invalid slot", GetName());
        return false;
    }
    m_textureSlot = data["slot"];

    if (!data.contains("path") || !data["path"].is_string())
    {
        LOG_ERROR("Texture material parameter {} has invalid path", GetName());
        return false;
    }
    m_textureHandle = ResourceManager::Get().LoadResource<TextureResource>(data["path"]);
    
    return true;
}
