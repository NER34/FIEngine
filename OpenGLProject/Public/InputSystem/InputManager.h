#pragma once

#include <queue>

#include "InputSystem/IInputManaget.h"

namespace FIE
{	
	class InputManager : public IInputManager
	{
	public:

		InputManager() {}

		void update() override;
		void setInputContext(InputContext* newInputContext) override;
		void updateInput(InputEvent inputEvent) override;
		void resetInputs() override;

	private:

		InputContext* m_inputContext = nullptr;
		std::queue<InputEvent> m_inputQueue;

	};
}