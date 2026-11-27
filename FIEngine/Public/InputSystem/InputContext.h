#pragma once

#include <unordered_map>

#include "IInputManaget.h"
#include "EventDispatcher/Dispatcher.h"
#include "InputSystem/InputDefines.h"


namespace FIE
{
	DECLARE_MULTI_DISPATCHER(InputHandle, InputIDs, InputValue)
	struct InputContextDesc
	{
		std::unordered_map<InputIDs, InputHandle> m_inputHandles;

		DECLARE_MULTI_DISPATCHER(TOnResetInput)
		TOnResetInput m_onResetInputs;
	};

	class InputContext
	{
	
	public:
		using InputWeekHandle = SingleDispatcher<InputIDs, InputValue>;

		InputContext() = default;

		void updateInput(InputEvent inputEvent);
		void resetInputs();

		DispatcherHandle assignHandle(InputIDs inputID, InputWeekHandle&& dispatcher);
		void removeHandle(InputIDs inputID, DispatcherHandle handleID);

	private:

		InputContextDesc m_props;
		
	};
}