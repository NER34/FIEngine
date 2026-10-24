#include "Rendering/Material/MaterialParameters/Matrix4Parameter.h"

#include "Rendering/Material/ShaderProgram.h"

void Matrix4Parameter::ApplyParameter(ShaderProgram* shaderProgram) const
{
    shaderProgram->SetUniformMat4(m_name, m_matrix);
}

bool Matrix4Parameter::Deserialize(const nlohmann::json& data)
{
    if (!IMaterialParameter::Deserialize(data))
    {
        return false;
    }

    if (!data.contains("value") || !data["value"].is_array() || data["value"].size() != 16)
    {
        LOG_ERROR("Matrix4 material parameter {} must contain sixteen values", GetName());
        return false;
    }

    for (std::size_t index = 0; index < 16; ++index)
    {
        if (!data["value"][index].is_number())
        {
            LOG_ERROR("Matrix4 material parameter {} has an invalid value", GetName());
            return false;
        }
        m_matrix[index % 4][index / 4] = data["value"][index];
    }
    
    return true;
}
