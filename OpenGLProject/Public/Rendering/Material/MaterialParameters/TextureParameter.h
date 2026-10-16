#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"

class Texture;

class TextureParameter : public MaterialParameterBase<TextureParameter>
{
public:
    
    TextureParameter() {};
    TextureParameter(const std::string& name, GLuint shaderProgramID, GLuint textureSlot, Texture* texture);
    
    void ApplyParameter() const override;
    
private:
    
    Texture* m_texture;
    GLuint m_textureSlot;
    
};
