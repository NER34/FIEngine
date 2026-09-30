#pragma once

#include <atomic>
#include <unordered_map>

#include "DispatcherEvents.h"

namespace FIE
{	
	namespace Private::DispatcherHandle
	{
		inline std::atomic<uint64_t> gNextID(1);
	}

	class DispatcherHandle
	{
		// class implementation was inspired from the one in Unreal Engine 5
	public:

		enum EGenerateNewHandle { NewHandle };

		DispatcherHandle() : id(0) {}
		explicit DispatcherHandle(EGenerateNewHandle)
			: id(GenerateNewID())
		{}

		bool IsValid() const { return id != 0; }
		void Reset() { id = 0; }

		bool operator==(const DispatcherHandle& Rhs) const
		{
			return id == Rhs.id;
		}

		bool operator!=(const DispatcherHandle& Rhs) const
		{
			return id != Rhs.id;
		}

		explicit operator uint64_t() const { return id; }

	private:

		uint64_t id;

		static uint64_t GenerateNewID()
		{
			uint64_t result = ++Private::DispatcherHandle::gNextID;
			if (result == 0)
			{
				// Increment it again - it might not be zero, so don't just assign it to 1.
				result = ++Private::DispatcherHandle::gNextID;
			}
			return result;
		}

	};

	template<typename... Ts>
	class SingleDispatcher
	{
	public:

		SingleDispatcher() = default;

		SingleDispatcher(const SingleDispatcher<Ts...>&) = delete;
		SingleDispatcher<Ts...>& operator=(const SingleDispatcher<Ts...>&) = delete;

		SingleDispatcher(SingleDispatcher<Ts...>&& other) noexcept
		{
			m_event = other.m_event;
			other.m_event = nullptr;
		}
		SingleDispatcher<Ts...>& operator=(SingleDispatcher<Ts...>&& other) noexcept
		{
			m_event = other.m_event;
			other.m_event = nullptr; 
			return *this;
		}

		template<typename TClass, typename TFunc>
		SingleDispatcher(TClass* inObject, TFunc inFuncPtr)
		{
			BindMemberFunction(inObject, inFuncPtr);
		}

		template<typename TFunc>
		SingleDispatcher(TFunc inFuncPtr)
		{
			BindStaticFunction(inFuncPtr);
		}

		template<typename TClass, typename TFunc>
		void BindMemberFunction(TClass* inObject, TFunc inFuncPtr)
		{
			using TEvent = MemberFunctionEvent<TClass, TFunc, Ts...>;
			return Bind_Internal<TEvent>(inObject, inFuncPtr);
		}

		template<typename TFunc>
		void BindStaticFunction(TFunc inFuncPtr)
		{
			using TEvent = StaticFunctionEvent<TFunc, Ts...>;
			Bind_Internal<TEvent>(inFuncPtr);
		}

		void Unbind()
		{
			if (IsBound())
			{
				delete m_event;
				m_event = nullptr;
			}
		}

		void Execute(Ts... args)
		{
			if (IsBound())
			{
				m_event->Execute(args...);
			}
		}

		bool IsBound() const
		{
			return m_event != nullptr;
		}

	private:

		template<typename TEventClass, typename... TParams>
		void Bind_Internal(TParams... params)
		{
			Unbind();
			m_event = new TEventClass(params...);
		}

		template<typename... Us> friend class Dispatcher;
		IDispatcherEvent<Ts...>* m_event = nullptr;

	};

	template<typename... Ts>
	class Dispatcher
	{
	public:

		Dispatcher() = default;

		Dispatcher(const Dispatcher<Ts...>&) = delete;
		Dispatcher<Ts...>& operator=(const Dispatcher<Ts...>&) = delete;

		Dispatcher(Dispatcher<Ts...>&& other) noexcept
		{
			m_events = std::move(other.m_events);
		}
		Dispatcher<Ts...>& operator=(Dispatcher<Ts...>&& other) noexcept
		{
			m_events = std::move(other.m_events);
			return *this;
		}

		template<typename TClass, typename TFunc>
		Dispatcher(TClass* inObject, TFunc inFuncPtr, DispatcherHandle& handle)
		{
			handle = BindMemberFunction(inObject, inFuncPtr);
		}

		template<typename TFunc>
		Dispatcher(TFunc inFuncPtr, DispatcherHandle& handle)
		{
			handle = BindStaticFunction(inFuncPtr);
		}

		Dispatcher(SingleDispatcher<Ts...>&& other, DispatcherHandle& handle)
		{
			handle = Bind(std::move(other));
		}
	
		~Dispatcher()
		{
			UnbindAll();
		}

		void Bind(Dispatcher<Ts...>&& other)
		{
			m_events.merge(std::move(other.m_events));
		}

		DispatcherHandle Bind(SingleDispatcher<Ts...>&& other)
		{
			if (!other.IsBound())
			{
				return DispatcherHandle();
			}

			DispatcherHandle ID(DispatcherHandle::EGenerateNewHandle::NewHandle);
			m_events.insert({ (uint64_t)ID, other.m_event });
			other.m_event = nullptr;
			return ID;
		}

		template<typename TClass, typename TFunc>
		DispatcherHandle BindMemberFunction(TClass* inObject, TFunc inFuncPtr)
		{
			using TEvent = MemberFunctionEvent<TClass, TFunc, Ts...>;
			return Bind_Internal<TEvent>(inObject, inFuncPtr);
		}

		template<typename TFunc>
		DispatcherHandle BindStaticFunction(TFunc inFuncPtr)
		{
			using TEvent = StaticFunctionEvent<TFunc, Ts...>;
			return Bind_Internal<TEvent>(inFuncPtr);
		}

		void Unbind(DispatcherHandle handle)
		{
			auto it = m_events.find((uint64_t)handle);
			if (it != m_events.end())
			{
				delete (*it).second;
				m_events.erase(it);
			}
		}

		void UnbindAll()
		{
			for (auto event : m_events)
			{
				delete event.second;
			}
			m_events.clear();
		}
	
		void Broadcast(Ts... args)
		{
			for (auto event : m_events)
			{
				event.second->Execute(args...);
			}
		}

		bool IsBound(DispatcherHandle handle) const
		{
			return m_events.find() != m_events.end();
		}

		bool IsAnyBound() const
		{
			return m_events.size() > 0;
		}
	
	private:

		template<typename TEventClass, typename... TParams>
		DispatcherHandle Bind_Internal(TParams... params)
		{
			DispatcherHandle ID(DispatcherHandle::EGenerateNewHandle::NewHandle);
			m_events.insert({ (uint64_t)ID, new TEventClass(params...) });
			return ID;
		}
	
		std::unordered_map<uint64_t, IDispatcherEvent<Ts...>*> m_events;
	};
}

#define DECLARE_MULTI_DISPATCHER(TDispatcherType, ...) \
typedef Dispatcher<__VA_ARGS__> TDispatcherType;