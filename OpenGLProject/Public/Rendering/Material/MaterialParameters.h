#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "MaterialParameter.h"
#include "Utilities/Logger.h"

class MaterialParameters
{
    using ParameterPtr = std::shared_ptr<IMaterialParameter>;
    using Storage = std::unordered_map<std::string, ParameterPtr>;

public:

    MaterialParameters() noexcept = default;

    MaterialParameters(const MaterialParameters& other) = delete;
    MaterialParameters& operator=(const MaterialParameters& other) = delete;

    MaterialParameters(MaterialParameters&& other) noexcept = delete;
    MaterialParameters& operator=(MaterialParameters&& other) noexcept = delete;

    ~MaterialParameters() = default;

    class const_iterator;

    class iterator
    {
        friend class MaterialParameters;
        friend class const_iterator;

    public:

        iterator() = default;

        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;
        using value_type = IMaterialParameter;
        using difference_type = std::ptrdiff_t;
        using reference = IMaterialParameter&;
        using pointer = IMaterialParameter*;

        reference operator*() const
        {
            return *m_iterator->second;
        }

        pointer operator->() const
        {
            return m_iterator->second.get();
        }

        iterator& operator++()
        {
            ++m_iterator;
            return *this;
        }

        iterator operator++(int)
        {
            iterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const iterator& other) const
        {
            return m_iterator == other.m_iterator;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

    private:

        explicit iterator(Storage::iterator iterator)
            : m_iterator(iterator)
        {}

        Storage::iterator m_iterator;
    };

    class const_iterator
    {
        friend class MaterialParameters;

    public:

        const_iterator() = default;

        using iterator_category = std::forward_iterator_tag;
        using iterator_concept = std::forward_iterator_tag;
        using value_type = IMaterialParameter;
        using difference_type = std::ptrdiff_t;
        using reference = const IMaterialParameter&;
        using pointer = const IMaterialParameter*;

        const_iterator(const iterator& other)
            : m_iterator(other.m_iterator)
        {}

        reference operator*() const
        {
            return *m_iterator->second;
        }

        pointer operator->() const
        {
            return m_iterator->second.get();
        }

        const_iterator& operator++()
        {
            ++m_iterator;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const const_iterator& other) const
        {
            return m_iterator == other.m_iterator;
        }

        bool operator!=(const const_iterator& other) const
        {
            return !(*this == other);
        }

    private:

        friend class iterator;

        explicit const_iterator(Storage::const_iterator iterator)
            : m_iterator(iterator)
        {}

        Storage::const_iterator m_iterator;
    };

    iterator begin() noexcept
    {
        return iterator(m_parameters.begin());
    }

    iterator end() noexcept
    {
        return iterator(m_parameters.end());
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(m_parameters.begin());
    }

    const_iterator end() const noexcept
    {
        return const_iterator(m_parameters.end());
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    template <typename TParam, typename... Args>
    TParam* PushParameter(const std::string& name, Args&&... args)
    {
        static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");

        auto parameter = std::make_shared<TParam>(std::forward<Args>(args)...);
        auto [it, inserted] = m_parameters.emplace(name, parameter);

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

    void ClearParameters()
    {
        m_parameters.clear();
    }

private:

    Storage m_parameters;
    
};
