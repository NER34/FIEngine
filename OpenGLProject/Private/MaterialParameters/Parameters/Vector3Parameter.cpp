#include "MaterialParameters/Parameters/Vector3Parameter.h"

#include "ResourceManager/Resources/ShaderProgramResource.h"

void Vector3Parameter::ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const
{
    shaderProgram->SetUniformFloat3(m_name, m_vector);
}

DEFINE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, Vector3Parameter)
{
    if (!data.contains("value") || !data["value"].is_array() || data["value"].size() != 3)
    {
        LOG_ERROR("Vector3 material parameter {} must contain three values", parameter.GetName());
        return false;
    }

    for (std::size_t index = 0; index < 3; ++index)
    {
        if (!data["value"][index].is_number())
        {
            LOG_ERROR("Vector3 material parameter {} has an invalid value", parameter.GetName());
            return false;
        }
        parameter.m_vector[index] = data["value"][index];
    }
    return true;
}

REGISTER_MATERIAL_PARAM_FACTORY(Vector3Parameter, Vector3Parameter::Deserialize)
