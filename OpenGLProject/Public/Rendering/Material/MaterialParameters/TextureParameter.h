#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"

class Texture;

class TextureParameter : public MaterialParameterBase<TextureParameter>
{
public:
    
    TextureParameter() {};
    TextureParameter(const std::string& name, Texture* texture, GLint textureSlot)
        : m_name(name), m_texture(texture), m_textureSlot(textureSlot) 
    {}
    
    void ApplyParameter(ShaderProgram* shaderProgram) const override;
    
private:
    
    std::string m_name;
    Texture* m_texture = nullptr;
    GLint m_textureSlot = 0;
    
};
