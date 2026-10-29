#include "Rendering/Material/MaterialParameters/Matrix4Parameter.h"

#include "ResourceManager/Resources/ShaderProgramResource.h"

void Matrix4Parameter::ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const
{
    shaderProgram->SetUniformMat4(m_name, m_matrix);
}

DEFINE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, Matrix4Parameter)
{
    if (!data.contains("value") || !data["value"].is_array() || data["value"].size() != 16)
    {
        LOG_ERROR("Matrix4 material parameter {} must contain sixteen values", parameter.GetName());
        return false;
    }

    for (std::size_t index = 0; index < 16; ++index)
    {
        if (!data["value"][index].is_number())
        {
            LOG_ERROR("Matrix4 material parameter {} has an invalid value", parameter.GetName());
            return false;
        }
        parameter.m_matrix[index % 4][index / 4] = data["value"][index];
    }
    
    return true;
}

REGISTER_MATERIAL_PARAM_FACTORY(Matrix4Parameter, Matrix4Parameter::Deserialize)
