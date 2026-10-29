#pragma once

#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "MaterialParameter.h"
#include "Utilities/Logger.h"

class MaterialParamContainer
{
    using ParameterPtr = std::unique_ptr<IMaterialParameter>;
    using Storage = std::unordered_map<std::string, ParameterPtr>;

public:

    MaterialParamContainer() noexcept = default;

    MaterialParamContainer(const MaterialParamContainer& other) = delete;
    MaterialParamContainer& operator=(const MaterialParamContainer& other) = delete;

    MaterialParamContainer(MaterialParamContainer&& other) noexcept = delete;
    MaterialParamContainer& operator=(MaterialParamContainer&& other) noexcept = delete;

    ~MaterialParamContainer() = default;

    template <typename TParam, typename... Args>
    TParam* PushParameter(const std::string& name, Args&&... args)
    {
        static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");

        auto parameter = std::make_unique<TParam>(std::forward<Args>(args)...);
        auto [it, inserted] = m_parameters.emplace(name, std::move(parameter));

        if (!inserted)
        {
            LOG_ERROR("Parameter with name {} already exists.", name);

            if (it->second->GetParameterType() != TParam::GetStaticParameterType())
            {
                LOG_ERROR("Parameter {} already has a different type.", name);
                return nullptr;
            }
        }

        return static_cast<TParam*>(it->second.get());
    }

    template <typename TParam>
    TParam* GetParameter(const std::string& name)
    {
        static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");

        IMaterialParameter* parameter = GetParameter(name);
        if (parameter == nullptr || parameter->GetParameterType() != TParam::GetStaticParameterType())
        {
            return nullptr;
        }

        return static_cast<TParam*>(parameter);
    }

    template <typename TParam>
    const TParam* GetParameter(const std::string& name) const
    {
        static_assert(
            std::is_base_of_v<IMaterialParameter, TParam>,
            "TParam must derive from IMaterialParameter");

        const IMaterialParameter* parameter = GetParameter(name);
        if (parameter == nullptr || parameter->GetParameterType() != TParam::GetStaticParameterType())
        {
            return nullptr;
        }

        return static_cast<const TParam*>(parameter);
    }

    IMaterialParameter* GetParameter(const std::string& name)
    {
        if (auto it = m_parameters.find(name); it != m_parameters.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    const IMaterialParameter* GetParameter(const std::string& name) const
    {
        if (auto it = m_parameters.find(name); it != m_parameters.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
    
    const auto& GetParameters() const noexcept
    {
        return m_parameters;
    }
    
    auto& GetParameters() noexcept
    {
        return m_parameters;
    }

    void ClearParameters()
    {
        m_parameters.clear();
    }

private:

    Storage m_parameters;
    
};
