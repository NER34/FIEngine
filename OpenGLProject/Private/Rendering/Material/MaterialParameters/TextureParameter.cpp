#include "Rendering/Material/MaterialParameters/TextureParameter.h"

#include "Rendering/Material/ShaderProgram.h"
#include "Rendering/Texture.h"


void TextureParameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    if (m_texture)
    {
        shaderProgram->SetUniformInt(m_name, m_textureSlot);
        m_texture->activate(GL_TEXTURE0 + m_textureSlot);
    }
}
