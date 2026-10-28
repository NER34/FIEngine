#pragma once
#include <string>

#include "glm/glm.hpp"
#include "Rendering/Material/MaterialParameter.h"

class Vector3Parameter : public MaterialParameterBase<Vector3Parameter>
{
public:
    
    void ApplyParameter(const ResourceHandle<ShaderProgramResource>& shaderProgram) const override;
    bool Deserialize(const nlohmann::json& data) override;
    
private:
    
    glm::vec3 m_vector;
    
};
