#pragma once

#include <vector>

#include "glad/glad.h"

#include "Shader.h"

struct MaterialDesc
{
    std::vector<ShaderDesc> m_shadersDesc;
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
    
    bool isValid() const;
    
protected:

    void createShaders(const MaterialDesc& desc);
    void createShaderProgram(const MaterialDesc& desc);
    void destroyMaterial();
    
private:

    GLuint m_shaderProgramID = 0;
    
    std::vector<Shader> m_shaders;
    
};
