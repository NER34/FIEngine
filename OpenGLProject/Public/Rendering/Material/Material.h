#pragma once

#include <string>
#include <vector>

#include "MaterialParametersPool.h"
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
    
    Material(const Material& other) = delete;/*= default*/;
    Material& operator=(const Material& other) = delete;/*= default*/;

    Material(Material&& other) noexcept = delete;
    Material& operator=(Material&& other) noexcept = delete;

    ~Material();
    
    void SetShaderProgram(const ShaderProgram* shaderProgram);
    
    void activate() const;
    void deactivate() const;
    
    GLuint getShaderProgramID() const;
    
    template <typename TParam/*, typename ... TArgs*/>
    MaterialParamHandle<TParam> PushParameter(const std::string& name, const TParam& param/*TArgs&& ... args*/)
    {
        MaterialParamHandle<TParam> handle = m_parametersPool.PushParameter<TParam>(param/*std::forward<TArgs>(args)...*/);
        if (handle.IsValid())
        {
            m_addedParameters[name] = handle;
        }
        return handle;
    }
    
    template <typename TParam/*, typename ... TArgs*/>
    MaterialParamHandle<TParam> PushParameter(const std::string& name, TParam&& param/*TArgs&& ... args*/)
    {        
        MaterialParamHandle<TParam> handle = m_parametersPool.PushParameter<TParam>(std::move(param)/*std::forward<TArgs>(args)...*/);
        if (handle.IsValid())
        {
            m_addedParameters[name] = handle;
        }
        return handle;
    }
    
    template <typename TParam>
    TParam* GetParameter(const std::string& name)
    {
        if (const auto it = m_addedParameters.find(name); it != m_addedParameters.end())
        {
            return m_parametersPool.GetParameter<TParam>(it->second);
        }
        return nullptr;
    }
    
    template <typename TParam>
    const TParam* GetParameter(const std::string& name) const
    {
        if (const auto it = m_addedParameters.find(name); it != m_addedParameters.end())
        {
            return m_parametersPool.GetParameter<TParam>(it->second);
        }
        return nullptr;
    }

    template <typename TParam>
    TParam* GetParameter(MaterialParamHandle<TParam> handle)
    {
        return m_parametersPool.GetParameter(handle);
    }

    template <typename TParam>
    const TParam* GetParameter(MaterialParamHandle<TParam> handle) const
    {
        return m_parametersPool.GetParameter(handle);
    }
    
    bool isValid() const;
    
protected:

    void clearMaterial();
    
private:
    
    std::unordered_map<std::string, MaterialParamHandleBase> m_addedParameters;
    const ShaderProgram* m_shaderProgram = nullptr;
    MaterialParametersPool m_parametersPool;
    
};
