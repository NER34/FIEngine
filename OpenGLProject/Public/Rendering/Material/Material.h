#pragma once

#include <string>
#include <vector>

#include "MaterialParameters.h"
#include "glad/glad.h"

#include "Rendering/Material/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "Utilities/Logger.h"

struct TextureSamplerDesc
{
    std::string m_name;
    const Texture* m_texture = nullptr;
};

struct MaterialDesc
{
    const ShaderProgram* m_shaderProgram = nullptr;
    std::vector<TextureSamplerDesc> m_texturesDesc;
};

class Material
{
public:

    Material() = default;
    
    Material(const Material& other) = delete;
    Material& operator=(const Material& other) = delete;

    Material(Material&& other) noexcept = delete;
    Material& operator=(Material&& other) noexcept = delete;

    ~Material();
    
    void SetShaderProgram(const ShaderProgram* shaderProgram);
    
    void activate() const;
    void deactivate() const;
    
    GLuint getShaderProgramID() const;
    
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
    
    bool isValid() const;
    
protected:

    void clearMaterial();
    
private:
    
    const ShaderProgram* m_shaderProgram = nullptr;
    MaterialParameters m_parameters;
    
};
