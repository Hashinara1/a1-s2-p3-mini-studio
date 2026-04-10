#pragma once

#include "InputManager.h"
#include "KeysBinding.h"

#include <unordered_map>
#include <set>
#include <algorithm>



template <typename T>
class ActionMap
{

private:
	InputManager& m_inputManager = InputManager::getInstance();
	std::unordered_map<T, std::vector<KeyBinding>> m_actions;


	bool checkInput(const std::pair<Input, TriggerState>& key) const
	{
		return std::visit([this, &key](auto val)
			{
				if constexpr (std::is_same_v<decltype(val), sf::Keyboard::Key>) {
					switch (key.second)
					{
					case (TriggerState::Down): return m_inputManager.isKeyDown(val);
					case (TriggerState::Held): return m_inputManager.isKeyHeld(val);
					case (TriggerState::Released): return m_inputManager.isKeyRelease(val);
					default: return false;
					}
				}
				else if constexpr (std::is_same_v<decltype(val), sf::Mouse::Button>)
				{
					switch (key.second)
					{
					case (TriggerState::Down): return m_inputManager.isMouseButtonDown(val);
					case (TriggerState::Held): return m_inputManager.isMouseButtonHeld(val);
					case (TriggerState::Released): return m_inputManager.isMouseButtonRelease(val);
					default: return false;
					}
				}
				else if constexpr (std::is_same_v<decltype(val), JoystickButton>)
				{
					switch ((int)val)
					{
					case(10):
						return sf::Joystick::getAxisPosition(m_inputManager.getControllerId(), sf::Joystick::Z) > 75;
					case(11):
						return sf::Joystick::getAxisPosition(m_inputManager.getControllerId(), sf::Joystick::Z) < -75;
					default:
						switch (key.second)
						{
						case (TriggerState::Down): return m_inputManager.isJoystickButtonDown(val);
						case (TriggerState::Held): return m_inputManager.isJoystickButtonHeld(val);
						case (TriggerState::Released): return m_inputManager.isJoystickButtonRelease(val);
						}
					}
					return false;
				}
				return false;
			},
			key.first);
	}

public:

	void bind(T _action, const std::vector<std::pair<Input, TriggerState>>& _cond)
	{
		KeyBinding kb;
		for (const std::pair<Input, TriggerState>& pair : _cond)
			kb.inputs.push_back(pair);
		m_actions[_action].push_back(kb);
	}

	const bool isActive(T _action) const
	{
		// explores all possible combos
		for (const auto& kb : m_actions.at(_action)) {
			bool valide = true;
			//We do all the touching inside
			for (const auto& duo : kb.inputs) {
				valide &= checkInput(duo);
			}
			if (valide)
				return true;
		}
		return false;
	}
};
