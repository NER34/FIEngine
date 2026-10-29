#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"
#include "Rendering/Material/MaterialParamFactory.h"

class ShaderProgramResource;

class FloatParameter : public MaterialParameterBase<FloatParameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, FloatParameter);
    
private:
    
    float m_value = 0.0f;
    
};
