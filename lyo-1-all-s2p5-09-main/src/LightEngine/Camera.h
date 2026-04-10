#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
private:

	sf::View m_view = sf::View({ 640.f, 360.f }, { 1280.f, 720.f });

public:

	void setPosition(const sf::Vector2f& _targetPos)
	{
		m_view.setCenter(_targetPos);
	}

	const sf::View& getView() const { return m_view; }

	void zoom(float _factor);

	void CorrectPosition(sf::Vector2f _bounds);

};