#pragma once
#include <string>

#include "glad/glad.h"
#include "Rendering/Material/MaterialParameter.h"

class FloatParameter : public MaterialParameterBase<FloatParameter/*, MaterialParamType::Float*/>
{
public:
    
    FloatParameter(const std::string& name, GLuint shaderProgramID, float value);
    
    void ApplyParameter() const override;
    
private:
    
    GLint m_location = -1;
    float m_value = 0.0f;
    
};
