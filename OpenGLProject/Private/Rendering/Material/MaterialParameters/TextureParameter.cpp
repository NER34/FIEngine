#include "Rendering/Material/MaterialParameters/TextureParameter.h"

#include "Rendering/Texture.h"
#include "Rendering/Validation.h"

TextureParameter::TextureParameter(
    const std::string& name, GLuint shaderProgramID, GLuint textureSlot, Texture* texture) 
    : m_texture(texture)
    , m_textureSlot(textureSlot)
{
    GLint uniformLoc = glGetUniformLocation(shaderProgramID, name.c_str());
    CHECK_OGL_ERR(glUniform1i(uniformLoc, textureSlot))
}

void TextureParameter::ApplyParameter()
{
    if (m_texture)
    {
        m_texture->activate(GL_TEXTURE0 + m_textureSlot);
    }
}
