#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct LightSource {

	sf::Color color;
	float radius;

	sf::Vector2f offset;
	bool isActive;
};
