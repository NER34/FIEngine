#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "MaterialParametersPool.h"
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Material/ShaderProgram.h"
#include "Rendering/Texture.h"
#include "Rendering/Validation.h"
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
    //Material(const MaterialDesc& desc);
    
    Material(const Material& other) = delete;/*= default*/;
    Material& operator=(const Material& other) = delete;/*= default*/;

    Material(Material&& other) noexcept = delete;
    Material& operator=(Material&& other) noexcept = delete;

    ~Material();
    
    void SetShaderProgram(const ShaderProgram* shaderProgram);
    
    //void setup(const MaterialDesc& desc);
    void activate() const;
    void deactivate() const;
    
    /*
    template <typename T>
    void setUniform(const std::string& name, const T& val) const
    {
        GLint uniformLoc = glGetUniformLocation(getShaderProgramID(), name.c_str());
        setUniform_Internal(uniformLoc, val);
    }
    */
    
    GLuint getShaderProgramID() const;
    
    template <typename TParam, typename ... TArgs>
    MaterialParamHandle<TParam> PushParameter(TArgs&& ... args)
    {
        return m_parametersPool.PushParameter<TParam>(std::forward<TArgs>(args)...);
    }
    
    template <typename TParam>
    TParam* GetParameter(MaterialParamHandle<TParam> handle) const
    {
        return m_parametersPool.GetParameter(handle);
    }
    
    bool isValid() const;
    
protected:

    //void setupTextureSamplers(const MaterialDesc& desc);
    void clearMaterial();
    
/*
private:
    
    template <typename T>
    void setUniform_Internal(GLint uniformLoc, const T& val) const
    {
        LOG_ERROR("Failed to set uniform: undefined type {}", typeid(val).name());
        assert(false);
    }
    
    template <>
    void setUniform_Internal(GLint uniformLoc, const glm::vec3& val) const
    {
        CHECK_OGL_ERR(glUniform3fv(uniformLoc, 1, glm::value_ptr(val)))
    }
    
    template <>
    void setUniform_Internal(GLint uniformLoc, const float& val) const
    {
        CHECK_OGL_ERR(glUniform1fv(uniformLoc, 1, &val))
    }
    
    template <>
    void setUniform_Internal(GLint uniformLoc, const glm::mat4& val) const
    {
        CHECK_OGL_ERR(glUniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(val)))
    }
    */
    
private:
    
    const ShaderProgram* m_shaderProgram = nullptr;
    MaterialParametersPool m_parametersPool;
    //std::vector<TextureSamplerDesc> m_textures;
    
};
