#pragma once

#include <optional>
#include <string>

#include "Rendering/Material/Shader.h"
#include "ResourceManager/ResourceBase.h"

class ShaderResource : public ResourceBase
{
public:
    
    ShaderResource() = default;
    ~ShaderResource() override;
    
    void Reload() override;
    void Free() override;
    
    const Shader* GetShader() const;
    bool IsValid() const;
    
private:
    
    std::optional<Shader> m_shader;
    
};
