#pragma once

#include <string>
#include <vector>

#include "MaterialParameters.h"
#include "glad/glad.h"

#include "Rendering/Material/ShaderProgram.h"
#include "ResourceManager/Resources/TextureResource.h"
#include "Utilities/Logger.h"


class Material
{
public:

    Material() = default;
    
    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept = delete;
    Material& operator=(Material&& other) noexcept = delete;

    ~Material();
    
    void SetShaderProgram(ShaderProgram* shaderProgram);
    ShaderProgram* GetShaderProgram() const;
    
    void Activate() const;
    void Deactivate() const;
    
    GLuint GetShaderProgramID() const;
    
    template <typename TParam>
    TParam* PushParameter(const std::string& name, const TParam& param)
    {
        return  m_parameters.PushParameter<TParam>(name, param);
    }
    
    template <typename TParam>
    TParam* PushParameter(const std::string& name, TParam&& param)
    {        
        return m_parameters.PushParameter<TParam>(name, std::move(param));
    }
    
    template <typename TParam>
    TParam* GetParameter(const std::string& name)
    {
        return m_parameters.GetParameter<TParam>(name);
    }
    
    template <typename TParam>
    const TParam* GetParameter(const std::string& name) const
    {
        return m_parameters.GetParameter<TParam>(name);
    }
    
    bool IsValid() const;
    
protected:

    void ClearMaterial();
    
private:
    
    ShaderProgram* m_shaderProgram = nullptr;
    MaterialParameters m_parameters;
    
};
