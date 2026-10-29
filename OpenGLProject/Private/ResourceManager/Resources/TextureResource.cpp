#include "ResourceManager/Resources/TextureResource.h"

#include <fstream>

#include "json.h"
#include "stb_image.h"
#include "Rendering/Validation.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"

TextureResource::~TextureResource()
{
    TextureResource::Free();
}

void TextureResource::Reload()
{
    Free();

    const std::string assetPath = GetFullFilePath();
    std::ifstream assetFile(assetPath);
    if (!assetFile.is_open())
    {
        LOG_ERROR("Failed to open texture asset file: {}", assetPath);
        assert(false);
        return;
    }

    nlohmann::json asset;
    try
    {
        assetFile >> asset;
    }
    catch (const nlohmann::json::exception& exception)
    {
        LOG_ERROR("Failed to parse texture asset file {}: {}", assetPath, exception.what());
        assert(false);
        return;
    }

    if (!asset.contains("type") || !asset["type"].is_string())
    {
        LOG_ERROR("Invalid asset file {}: type is missing", GetFilePath());
        assert(false);
        return;
    }
    if (asset["type"] != "TextureResource")
    {
        LOG_ERROR("Invalid asset file {}: expected type {}, got {}", GetFilePath(), "TextureResource", asset["type"].get<std::string>());
        assert(false);
        return;
    }

    if (!asset.contains("path") || !asset["path"].is_string())
    {
        LOG_ERROR("Invalid texture asset file {}: path is missing", assetPath);
        assert(false);
        return;
    }
    
    std::string texturePath = (GetResourcesPath() / asset["path"].get<std::string>()).string();
    TextureDesc textureDesc;
    textureDesc.data = stbi_load(
        texturePath.c_str(), 
        &textureDesc.dimensions[0], &textureDesc.dimensions[1], 
        &textureDesc.numChannels, 0
        );
    
    if (textureDesc.data)
    {
        CreateTexture(textureDesc);
        stbi_image_free(textureDesc.data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: {}", texturePath);
        assert(false);
    }
}

void TextureResource::Free()
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glDeleteTextures(1, &m_textureID))
        m_textureID = 0;
    }
}

void TextureResource::Activate(GLenum textureUnit) const
{
    if (IsValid())
    {
        CHECK_OGL_ERR(glActiveTexture(textureUnit))
        CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, m_textureID))
    }
    else
    {
        LOG_ERROR("Failed activate texture: m_textureID is invalid");
    }
}

bool TextureResource::IsValid() const
{
    return m_textureID != 0;
}

GLuint TextureResource::GetTextureID() const
{
    return m_textureID;
}

void TextureResource::CreateTexture(const TextureDesc& desc)
{
    CHECK_OGL_ERR(glGenTextures(1, &m_textureID))
    CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, m_textureID))
    
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    CHECK_OGL_ERR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR))
    
    GLint format = GL_RGB;
    if (desc.numChannels == 1)
    {
        format = GL_RED;
    }
    else if (desc.numChannels == 4)
    {
        format = GL_RGBA;
    }
    
    CHECK_OGL_ERR(glTexImage2D(
        GL_TEXTURE_2D, 0, format, 
        desc.dimensions[0], desc.dimensions[1], 
        0, format, GL_UNSIGNED_BYTE, desc.data
        ))
    CHECK_OGL_ERR(glGenerateMipmap(GL_TEXTURE_2D))

    CHECK_OGL_ERR(glBindTexture(GL_TEXTURE_2D, 0))
}
