#pragma once

#include <queue>

#include "InputSystem/IInputManaget.h"
#include "SystemBase/SystemLocator.h"

namespace FIE
{	
	class InputManager : public IInputManager, public ISystemBase
	{
	public:

		InputManager() {}
		
		void Initialize() override;
		void Update(float deltaTime) override;

		void update() override;
		void setInputContext(InputContext* newInputContext) override;
		InputContext* getInputContext() override;
		void updateInput(InputEvent inputEvent) override;
		void resetInputs() override;

	private:

		InputContext* m_inputContext = nullptr;
		std::queue<InputEvent> m_inputQueue;
	};
	
}
