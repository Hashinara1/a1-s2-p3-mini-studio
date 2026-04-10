#include "Camera.h"
#include "GameManager.h"

void Camera::zoom(float _factor)
{
	m_view.zoom(_factor);
}

void Camera::CorrectPosition(sf::Vector2f _bounds)
{
	sf::Vector2f size = m_view.getSize();
	sf::Vector2f midSize = sf::Vector2f(size.x / 2, size.y / 2);
	sf::Vector2f pos = m_view.getCenter();


	if (pos.x - midSize.x < 0)
		pos.x = midSize.x;

	if (pos.x + midSize.x > _bounds.x)
		pos.x = _bounds.x - midSize.x;

	if (pos.y - midSize.y < 0)
		pos.y = midSize.y;

	if (pos.y + midSize.y > _bounds.y)
		pos.y = _bounds.y - midSize.y;

	m_view.setCenter(pos);
}
