#include "Rendering/Material/MaterialParameters/FloatParameter.h"

#include "Rendering/Material/ShaderProgram.h"
#include "Utilities/Logger.h"

void FloatParameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    shaderProgram->SetUniformFloat(m_name, m_value);
}

bool FloatParameter::Deserialize(const nlohmann::json& data)
{
    if (!IMaterialParameter::Deserialize(data))
    {
        return false;
    }
    
    if (!data.contains("value") || !data["value"].is_number())
    {
        LOG_ERROR("Float material parameter {} has an invalid value", GetName());
    }
    m_value = data["value"];
    
    return true;
}
