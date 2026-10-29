#pragma once
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"
#include "Rendering/Material/MaterialParamFactory.h"

class Matrix4Parameter : public MaterialParameterBase<Matrix4Parameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, Matrix4Parameter);
    
private:
    
    glm::mat4 m_matrix;
    
};
