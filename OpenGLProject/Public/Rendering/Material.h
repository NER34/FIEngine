#pragma once

#include <vector>

#include "glad/glad.h"

#include "Shader.h"
#include "Texture.h"
#include "Validation.h"
#include "glm/vec2.hpp"
#include "glm/gtc/type_ptr.hpp"
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
        glUniform3fv(uniformLoc, 1, glm::value_ptr(val));
        //CHECK_OGL_ERR("Material glUniform3fv")
    }
    
    template <>
    void setUniform_Internal(GLint uniformLoc, const float& val) const
    {
        glUniform1fv(uniformLoc, 1, &val);
        //CHECK_OGL_ERR("Material glUniform1fv")
    }
    
    template <>
    void setUniform_Internal(GLint uniformLoc, const glm::mat4& val) const
    {
        glUniformMatrix4fv(uniformLoc, 1, false, glm::value_ptr(val));
        //CHECK_OGL_ERR("Material glUniformMatrix4fv")
    }
    
private:

    GLuint m_shaderProgramID = 0;
    
    std::vector<Shader> m_shaders;
    std::vector<Texture> m_textures;
    
};
