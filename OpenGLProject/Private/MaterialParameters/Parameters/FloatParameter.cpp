#include "MaterialParameters/Parameters/FloatParameter.h"

#include "ResourceManager/Resources/ShaderProgramResource.h"
#include "Utilities/Logger.h"

void FloatParameter::ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const
{
    shaderProgram->SetUniformFloat(m_name, m_value);
}

DEFINE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, FloatParameter)
{
    if (!data.contains("value") || !data["value"].is_number())
    {
        LOG_ERROR("Float material parameter {} has an invalid value", parameter.GetName());
        return false;
    }
    parameter.m_value = data["value"];
    
    return true;
}

REGISTER_MATERIAL_PARAM_FACTORY(FloatParameter, FloatParameter::Deserialize)
