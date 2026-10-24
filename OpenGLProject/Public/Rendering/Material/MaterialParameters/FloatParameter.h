#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"

class ShaderProgram;

class FloatParameter : public MaterialParameterBase<FloatParameter>
{
public:
    
    void ApplyParameter(ShaderProgram* shaderProgram) const override;
    bool Deserialize(const nlohmann::json& data) override;
    
private:
    
    float m_value = 0.0f;
    
};
