#pragma once

#include <optional>
#include <vector>

#include "json.h"
#include "Rendering/Material/Material.h"
#include "ResourceManager/ResourceBase.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ShaderProgramResource.h"
#include "ResourceManager/Resources/TextureResource.h"

class MaterialResource : public ResourceBase
{
public:

    MaterialResource() = default;
    ~MaterialResource() override;

    void Reload() override;
    void Free() override;

    const Material* GetMaterial() const;
    Material* GetMaterial();
    bool IsValid() const;

private:
    
    bool AddParameter(const ShaderProgram* shaderProgram, const nlohmann::json& json);
    
    std::optional<Material> m_material;
    ResourceHandle<ShaderProgramResource> m_shaderProgramResource;
};
