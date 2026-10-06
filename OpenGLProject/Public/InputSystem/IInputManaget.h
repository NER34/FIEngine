#pragma once

#include "InputSystem/InputDefines.h"

namespace FIE
{
	class InputContext;
	
	struct InputEvent
	{
		InputIDs m_inputID;
		InputValue m_inputValue;
	};

	class IInputManager
	{
	public:
		
		virtual ~IInputManager() = default;

		static IInputManager& Get();

		virtual void update() = 0;
		virtual void setInputContext(InputContext* newInputContext) = 0;
		virtual InputContext* getInputContext() = 0;
		virtual void updateInput(InputEvent inputEvent) = 0;
		virtual void resetInputs() = 0;
	};
	
}