#include "GameManager.h"
#include "InputManager.h"

#include <iostream>

#include <SFML/Graphics.hpp>


InputManager::InputManager()
{
	sf::Joystick::update();
	for (unsigned int i = 0; i < sf::Joystick::Count; i++)
	{
		if (sf::Joystick::isConnected(i) &&
			sf::Joystick::getIdentification(i).name.find("Controller (Xbox One For Windows)") != std::string::npos)
		{
			m_controllerId = i;
			std::cout << "Controller found at index " << i << std::endl;
			break;
		}
	}
	if (m_controllerId == -1)
		std::cout << "Controller not found" << std::endl;
	m_controllerId = 0;
}

void InputManager::update()
{


	//updating input statuses
	for (auto& paire : m_keysStates) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }
	}
	for (auto& paire : m_mouseStates) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }
	}

	for (auto& paire : m_joystickState) {
		if (paire.second.isDown) { paire.second.isDown = false; paire.second.isHeld = true; }
		if (paire.second.isRelease) { paire.second.isRelease = false; paire.second.isHeld = false; }

	}


	sf::Event event;
	while (GameManager::Get()->mpWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			GameManager::Get()->mpWindow->close();
			break;

		case sf::Event::Resized:
		case sf::Event::LostFocus:
		case sf::Event::GainedFocus:
		case sf::Event::MouseEntered:
		case sf::Event::MouseLeft:
			GameManager::Get()->mpScene->OnWindowEvent(event);
			break;


		case sf::Event::KeyPressed:

			if (m_keysStates[event.key.code].isHeld)
				break;

			m_keysStates[event.key.code] = { true, false, false };
			break;

		case sf::Event::KeyReleased:
			m_keysStates[event.key.code] = { false, false, true };
			break;

		case sf::Event::MouseButtonPressed:
			if (m_mouseStates[event.mouseButton.button].isHeld)
				break;

			m_mouseStates[event.mouseButton.button] = { true, false, false };
			break;

		case sf::Event::MouseButtonReleased:
			m_mouseStates[event.mouseButton.button] = { false, false, true };
			break;

		case sf::Event::MouseMoved:
			m_mousePos = { event.mouseMove.x, event.mouseMove.y };
			break;

		default:
			//There are still quite a few events left; shift-click on sf::Event to see them.
			break;
		}
	}

	for ( int i = 0; i < 12; i++) {
		if (sf::Joystick::isButtonPressed(m_controllerId, i)) {
			if (m_joystickState[(JoystickButton)(i)].isHeld) {
				continue;
			}
			m_joystickState[(JoystickButton)(i)] = { true, false, false };
		}
		else 
			if (m_joystickState[(JoystickButton)(i)].isHeld || m_joystickState[(JoystickButton)(i)].isDown)
				m_joystickState[(JoystickButton)(i)] = { false, false, true };
	}
}

bool InputManager::isKeyDown(const sf::Keyboard::Key _key) const
{
	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isKeyHeld(const sf::Keyboard::Key _key) const
{

	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isKeyRelease(const sf::Keyboard::Key _key) const
{

	if (auto it = m_keysStates.find(_key); it != m_keysStates.end())
		return it->second.isRelease;
	return false;
}

bool InputManager::isMouseButtonDown(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isMouseButtonHeld(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isMouseButtonRelease(const sf::Mouse::Button _btn) const
{
	if (auto it = m_mouseStates.find(_btn); it != m_mouseStates.end())
		return it->second.isRelease;
	return false;
}

bool InputManager::isJoystickButtonDown(const JoystickButton _btn) const
{
	if (auto it = m_joystickState.find(_btn); it != m_joystickState.end())
		return it->second.isDown;
	return false;
}

bool InputManager::isJoystickButtonHeld(const JoystickButton _btn) const
{
	if (auto it = m_joystickState.find(_btn); it != m_joystickState.end())
		return it->second.isHeld;
	return false;
}

bool InputManager::isJoystickButtonRelease(const JoystickButton _btn) const
{
	if (auto it = m_joystickState.find(_btn); it != m_joystickState.end())
		return it->second.isRelease;
	return false;
}



float InputManager::getAxisValue(JoystickAxis _axis) const
{
	if (m_controllerId == -1) return 0.f;
	return sf::Joystick::getAxisPosition(m_controllerId, (sf::Joystick::Axis)_axis);
}