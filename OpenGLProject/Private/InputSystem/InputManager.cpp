
#include "InputSystem/InputManager.h"
#include "InputSystem/InputContext.h"
#include "Window/WindowManager.h"
#include "easy/profiler.h"

namespace FIE
{
	IInputManager& IInputManager::Get()
	{
		auto* system = SystemLocator::Get().GetSystem<IInputManager>();
		assert(system && "IInputManager is not registered");
		return *static_cast<InputManager*>(system);
	}

	void InputManager::Initialize()
	{
		EASY_FUNCTION();

		static InputContext inputContext;
		setInputContext(&inputContext);
	
		inputContext.assignHandle(KEY_ESCAPE, [](InputIDs, float)
		{
			if (auto* system = SystemLocator::Get().GetSystem<WindowManager>())
			{
				auto& windowManager = *static_cast<WindowManager*>(system);
				windowManager.GetWindow()->requestWindowClose();
			}
		});
	}

	void InputManager::Update(float deltaTime)
	{
		EASY_FUNCTION();

		update();
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

	InputContext* InputManager::getInputContext()
	{
		return m_inputContext;
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
