#include "Rendering/Material/MaterialParameters/Vector3Parameter.h"

#include "ResourceManager/Resources/ShaderProgramResource.h"

void Vector3Parameter::ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const
{
    shaderProgram->SetUniformFloat3(m_name, m_vector);
}

bool Vector3Parameter::Deserialize(const nlohmann::json& data)
{
    if (!IMaterialParameter::Deserialize(data))
    {
        return false;
    }
    
    if (!data.contains("value") || !data["value"].is_array() || data["value"].size() != 3)
    {
        LOG_ERROR("Vector3 material parameter {} must contain three values", GetName());
        return false;
    }

    for (std::size_t index = 0; index < 3; ++index)
    {
        if (!data["value"][index].is_number())
        {
            LOG_ERROR("Vector3 material parameter {} has an invalid value", GetName());
            return false;
        }
        m_vector[index] = data["value"][index];
    }
    return true;
}
