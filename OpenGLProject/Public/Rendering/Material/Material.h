#pragma once

#include <vector>

#include "glad/glad.h"

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Rendering/Texture.h"
#include "Rendering/Validation.h"
#include "Utilities/Logger.h"

struct TextureSamplerDesc
{
    std::string m_name;
    TextureDesc m_textureDesc;
};

struct MaterialDesc
{
    std::vector<ShaderDesc> m_shadersDesc;
    std::vector<TextureSamplerDesc> m_texturesDesc;
};

class Material
{
public:

    Material() = default;
    Material(const MaterialDesc& desc);
    
    Material(const Material& other) = default;
    Material& operator=(const Material& other) = default;

    Material(Material&& other) noexcept;
    Material& operator=(Material&& other) noexcept;

    ~Material();
    
    void setup(const MaterialDesc& desc);
    void activate() const;
    void deactivate() const;
    
    template <typename T>
    void setUniform(const std::string& name, const T& val) const
    {
        //glUseProgram(m_shaderProgramID);
        GLint uniformLoc = glGetUniformLocation(m_shaderProgramID, name.c_str());
        setUniform_Internal(uniformLoc, val);
        //glUseProgram(0);
    }
    
    GLuint getShaderProgramID() const;
    
    bool isValid() const;
    
protected:

    void createShaders(const MaterialDesc& desc);
    void createShaderProgram(const MaterialDesc& desc);
    void createTextures(const MaterialDesc& desc);
    void setupTextureSamplers(const MaterialDesc& desc);
    void destroyMaterial();
    
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
    
private:

    GLuint m_shaderProgramID = 0;
    
    std::vector<Shader> m_shaders;
    std::vector<Texture> m_textures;
    
};
