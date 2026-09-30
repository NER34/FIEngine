
#include "InputSystem/InputContext.h"
#include "InputSystem/InputManager.h"

namespace FIE
{
	void InputContext::resetInputs()
	{
		m_props.m_onResetInputs.Broadcast();
	}

	void InputContext::updateInput(InputEvent inputEvent)
	{
		auto it = m_props.m_inputHandles.find(inputEvent.m_inputID);
		if (it != m_props.m_inputHandles.end())
		{
			(*it).second.Broadcast(inputEvent.m_inputValue);
		}
	}


	DispatcherHandle InputContext::assignHandle(InputIDs inputID, InputWeekHandle&& dispatcher)
	{
		auto it = m_props.m_inputHandles.find(inputID);
		if (it == m_props.m_inputHandles.end())
		{
			DispatcherHandle handle;
			InputHandle tmpDispatcher(std::move(dispatcher), handle);
			m_props.m_inputHandles.insert(std::make_pair(inputID, std::move(tmpDispatcher)));
			return handle;
		}
		else
		{
			return (*it).second.Bind(std::move(dispatcher));
		}
	}

	void InputContext::removeHandle(InputIDs inputID, DispatcherHandle handleID)
	{
		auto it = m_props.m_inputHandles.find(inputID);
		if (it != m_props.m_inputHandles.end())
		{
			InputHandle& handle = (*it).second;
			handle.Unbind(handleID);
			if (!handle.IsAnyBound())
			{
				m_props.m_inputHandles.erase(inputID);
			}
		}
	}
}