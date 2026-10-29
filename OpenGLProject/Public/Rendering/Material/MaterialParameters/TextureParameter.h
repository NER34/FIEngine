#pragma once

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"
#include "Rendering/Material/MaterialParamFactory.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/TextureResource.h"

class TextureParameter : public MaterialParameterBase<TextureParameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, TextureParameter);

private:
    
    ResourceHandle<TextureResource> m_textureHandle;
    GLint m_textureSlot = 0;
    
};
