#pragma once

#include <string>

#include "glad/glad.h"
#include "json.h"
#include "Rendering/Material/MaterialParamContainer.h"
#include "ResourceManager/ResourceBase.h"
#include "ResourceManager/ResourceHandle.h"
#include "ResourceManager/Resources/ShaderProgramResource.h"

class MaterialResource : public ResourceBase
{
public:

    MaterialResource() = default;
    ~MaterialResource() override;

    MaterialResource(const MaterialResource& other) = delete;
    MaterialResource& operator=(const MaterialResource& other) = delete;

    void Reload() override;
    void Free() override;

    void Activate() const;
    void Deactivate() const;

    bool IsValid() const;

    const ResourceHandle<ShaderProgramResource>& GetShaderProgram() const;
    GLuint GetShaderProgramID() const;

    /*
    template <typename TParam>
    TParam* PushParameter(const std::string& name, const TParam& param)
    {
        return m_parameters.PushParameter<TParam>(name, param);
    }

    template <typename TParam>
    TParam* PushParameter(const std::string& name, TParam&& param)
    {
        return m_parameters.PushParameter<TParam>(name, std::move(param));
    }
    */

    template <typename TParam, typename... TArgs>
    TParam* PushParameter(const std::string& name, TArgs&&... args)
    {
        return m_parameters.PushParameter<TParam>(name, std::forward<TArgs>(args)...);
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

private:

    bool AddParameter(const nlohmann::json& json);

    ResourceHandle<ShaderProgramResource> m_shaderProgram;
    MaterialParamContainer m_parameters;

};
