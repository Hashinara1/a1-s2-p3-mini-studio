#include "UI.h"

template<typename T>
std::shared_ptr<Entity> UI::CreateSharedEntity(float radius, const sf::Color& color)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	std::shared_ptr<Entity> newEntity(new T);

	Entity* entity = newEntity.get();
	entity->Initialize(radius, color);

	m_entities.push_back(newEntity);

	return newEntity;
}

template<typename T>
std::shared_ptr<Entity> UI::CreateSharedEntity(sf::Vector2f size, const sf::Color& color)
{
	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");

	std::shared_ptr<Entity> newEntity = std::make_shared<T>();

	Entity* entity = newEntity.get();
	entity->Initialize(size, color);

	m_entities.push_back(newEntity);

	return newEntity;
}