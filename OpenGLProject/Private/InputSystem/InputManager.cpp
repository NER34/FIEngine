
#include "InputSystem/InputManager.h"
#include "InputSystem/InputContext.h"

namespace FIE
{
	IInputManager& IInputManager::Get()
	{
		static InputManager instance;
		return instance;
	}

	void InputManager::update()
	{
		while (m_inputQueue.size() > 0)
		{
			auto& inputEvent = m_inputQueue.front();
			if (m_inputContext != nullptr)
			{
				m_inputContext->updateInput(inputEvent);
			}
			m_inputQueue.pop();
		}
	}

	void InputManager::setInputContext(InputContext* newInputContext)
	{
		m_inputContext = newInputContext;
	}

	void InputManager::updateInput(InputEvent inputEvent)
	{
		m_inputQueue.push(inputEvent);
	}

	void InputManager::resetInputs()
	{
		m_inputContext->resetInputs();
	}
}