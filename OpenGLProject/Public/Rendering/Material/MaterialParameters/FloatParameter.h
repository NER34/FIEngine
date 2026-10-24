#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"

class ShaderProgram;

class FloatParameter : public MaterialParameterBase<FloatParameter>
{
public:
    
    FloatParameter() {}
    FloatParameter(const std::string& name, float value)
        : m_name(name), m_value(value)
    {}
    
    void ApplyParameter(ShaderProgram* shaderProgram) const override;
    
private:
    
    std::string m_name;
    float m_value = 0.0f;
    
};
