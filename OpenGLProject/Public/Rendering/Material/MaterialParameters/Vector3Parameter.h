#pragma once
#include <string>

#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"
#include "Rendering/Material/MaterialParamFactory.h"

class Vector3Parameter : public MaterialParameterBase<Vector3Parameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, Vector3Parameter);
    
private:
    
    glm::vec3 m_vector;
    
};
