#pragma once
#include <functional>

namespace FIE
{
	template<typename... Ts>
	class IDispatcherEvent
	{
	public:
		virtual void Execute(Ts...) = 0;
	};
	
	template<typename TClass, typename TFunc, typename... Ts>
	class MemberFunctionEvent : public IDispatcherEvent<Ts...>
	{
	public:

		MemberFunctionEvent(TClass* inObject, TFunc inFuncPtr)
			: object(inObject)
			, funcPtr(inFuncPtr)
		{}
	
		virtual void Execute(Ts... args) override
		{
			(object->*funcPtr)(args...);
		}
	
	private:
	
		TClass* object = nullptr;
		TFunc funcPtr;
	};

	template<typename TFunc, typename... Ts>
	class StaticFunctionEvent : public IDispatcherEvent<Ts...>
	{
	public:

		StaticFunctionEvent(TFunc inFuncPtr)
			: funcPtr(inFuncPtr)
		{
		}

		virtual void Execute(Ts... args) override
		{
			funcPtr(args...);
		}

	private:

		TFunc funcPtr;
	};

}
