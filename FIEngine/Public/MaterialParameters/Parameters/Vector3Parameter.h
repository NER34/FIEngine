#pragma once
#include <string>

#include "glm/glm.hpp"
#include "MaterialParameters/MaterialParameter.h"
#include "MaterialParameters/MaterialParamFactory.h"

class Vector3Parameter : public MaterialParameterBase<Vector3Parameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;

    DECLARE_MATERIAL_PARAM_DESERIALIZE_FUNC(Deserialize, Vector3Parameter);
    
private:
    
    glm::vec3 m_vector;
    
};
