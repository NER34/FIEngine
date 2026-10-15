#pragma once

#include <unordered_map>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <utility>

#include "MaterialParameter.h"

class MaterialParamHandleBase
{
    friend class MaterialParametersPool;
    uint32_t m_offset = UINT32_MAX;
    
    bool operator==(const MaterialParamHandleBase& other) const
    {
        return m_offset == other.m_offset;
    }
    
    bool IsValid() const
    {
        return m_offset != UINT32_MAX;
    }
};

template <typename TParam>
class MaterialParamHandle : public MaterialParamHandleBase
{
    static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");
};

class MaterialParametersPool
{    
public:

    MaterialParametersPool() noexcept = default;
    
    MaterialParametersPool(const MaterialParametersPool& other) = delete;
    MaterialParametersPool& operator=(const MaterialParametersPool& other) = delete;

    MaterialParametersPool(MaterialParametersPool&& other) noexcept = delete;
    MaterialParametersPool& operator=(MaterialParametersPool&& other) noexcept = delete;
    
    ~MaterialParametersPool()
    {
        ClearParameters();
        ::operator delete(m_buffer);
    }

    class iterator
    {
        friend class MaterialParametersPool;

    public:
        
        IMaterialParameter* operator*() const
        {
            return m_pool->GetParameter(m_handle);
        }

        IMaterialParameter* operator->() const
        {
            return m_pool->GetParameter(m_handle);
        }

        iterator& operator++()
        {
            m_handle = m_pool->NextHandle(m_handle);
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
            return m_pool == other.m_pool && m_handle == other.m_handle;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

    private:
        
        iterator(MaterialParametersPool* pool, MaterialParamHandleBase handle)
            : m_pool(pool)
            , m_handle(handle)
        {}

        MaterialParametersPool* m_pool = nullptr;
        MaterialParamHandleBase m_handle;
    };

    iterator begin() noexcept
    {
        if (m_size > 0)
        {
            MaterialParamHandleBase handle;
            handle.m_offset = 0;
            return iterator(this, handle);
        }
        return end();
    }

    iterator end() noexcept
    {
        return iterator(this, MaterialParamHandleBase());
    }
    
    void Reserve(uint32_t newCapacity)
    {
        if (newCapacity <= m_capacity)
        {
            return;
        }
        
        uint8_t* newBuffer = (uint8_t*)::operator new(newCapacity * sizeof(uint8_t));
        
        uint32_t offset = 0;
        while (offset < m_size)
        {
            auto* param = reinterpret_cast<IMaterialParameter*>(m_buffer + offset);
            uint32_t paramSize = param->GetParameterSize();
            
            param->MoveParameter(newBuffer + offset);
            std::destroy_at(param);
            
            offset += paramSize;
        }
        
        ::operator delete(m_buffer);
        m_buffer = newBuffer;
        m_capacity = newCapacity;        
    }
    
    template <typename TParam, typename... Args>
    MaterialParamHandle<TParam> PushParameter(Args&& ... args)
    {
        static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");
    
        if (m_capacity - sizeof(TParam) < m_size)
        {
            constexpr uint32_t initPoolCapacity = sizeof(TParam);
            uint32_t newCapacity = m_capacity ? m_capacity : initPoolCapacity;
            while (newCapacity - sizeof(TParam) < m_size)
            {
                // TODO: fix overflow
                newCapacity *= 2;
            }
            Reserve(newCapacity);
        }
        
        MaterialParamHandle<TParam> handle;
        handle.m_offset = m_size;
        m_paramHandles.insert({ handle.m_offset, TParam::GetStaticParameterType() });
        // TODO: try/catch to handle exceptions?
        new (m_buffer + handle.m_offset) TParam(std::forward<Args>(args)...);
        m_size += sizeof(TParam);
        
        return handle;
    }
    
    bool IsValidHandle(MaterialParamHandleBase handle)
    {
        if (handle.IsValid())
        {
            if (auto it = m_paramHandles.find(handle.m_offset); it != m_paramHandles.end())
            {
                return true;
            }
        }
        return false;
    }
    
    MaterialParamHandleBase NextHandle(MaterialParamHandleBase handle)
    {
        if (handle.IsValid())
        {
            if (IMaterialParameter* param = GetParameter(handle))
            {
                handle.m_offset += param->GetParameterSize();
                return IsValidHandle(handle) ? handle : MaterialParamHandleBase();
            }
        }
        return MaterialParamHandleBase();
    }
    
    template <typename TParam>
    TParam* GetParameter(MaterialParamHandle<TParam> handle)
    {
        static_assert(std::is_base_of_v<IMaterialParameter, TParam>, "TParam must derive from IMaterialParameter");
        
        if (handle.m_offset <= m_size && sizeof(TParam) <= m_size - handle.m_offset)
        {
            auto it = m_paramHandles.find(handle.m_offset);
            if (it != m_paramHandles.end() && it->second == TParam::GetStaticParameterType())
            {
                return reinterpret_cast<TParam*>(m_buffer + handle.m_offset);
            }
        }
        return nullptr;
    }
    
    IMaterialParameter* GetParameter(MaterialParamHandleBase handle)
    {
        if (IsValidHandle(handle))
        {
            return reinterpret_cast<IMaterialParameter*>(m_buffer + handle.m_offset);
        }
        return nullptr;
    }
    
    void ClearParameters()
    {
        uint32_t offset = 0;
        while (offset < m_size)
        {
            auto* param = reinterpret_cast<IMaterialParameter*>(m_buffer + offset);
            offset += param->GetParameterSize();
            
            std::destroy_at(param);
        }
        m_size = 0;
        m_paramHandles.clear();
    }
    
private:

    /*
    IMaterialParameter* ParameterAt(int32_t offset) const
    {
        if (m_buffer == nullptr || offset < 0 || offset >= m_size)
        {
            return nullptr;
        }

        return reinterpret_cast<IMaterialParameter*>(m_buffer + offset);
    }

    int32_t NextOffset(int32_t offset) const
    {
        auto* parameter = ParameterAt(offset);
        return offset + static_cast<int32_t>(parameter->GetParameterSize());
    }
    */
    
    std::unordered_map<uint32_t, std::type_index> m_paramHandles;
    uint8_t* m_buffer = nullptr;
    int32_t m_size = 0;
    int32_t m_capacity = 0;
    
};
