#pragma once

#include "MaterialParameters/MaterialParameter.h"
#include "MaterialParameters/MaterialParamFactory.h"

class ShaderProgramResource;

class FloatParameter : public MaterialParameterBase<FloatParameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, FloatParameter);
    
private:
    
    float m_value = 0.0f;
    
};
