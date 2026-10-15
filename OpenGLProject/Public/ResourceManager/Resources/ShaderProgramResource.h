#pragma once

#include <optional>
#include <vector>

#include "Rendering/Material/ShaderProgram.h"
#include "ResourceManager/ResourceBase.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ShaderResource.h"

class ShaderProgramResource : public ResourceBase
{
public:
    
    ShaderProgramResource() = default;
    ~ShaderProgramResource() override;
    
    void Reload() override;
    void Free() override;
    
    const ShaderProgram* GetShaderProgram() const;

private:
    
    std::optional<ShaderProgram> m_shaderProgram;
    std::vector<ResourceHandle<ShaderResource>> m_shaderResources;
    
};
