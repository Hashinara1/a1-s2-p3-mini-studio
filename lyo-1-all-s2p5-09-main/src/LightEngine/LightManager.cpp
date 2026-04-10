#include "Entity.h"
#include "LightManager.h"

constexpr float kPi = 3.14159265f;

void LightManager::update()
{

	for (Entity* entity : GameManager::Get()->mEntitiesToDestroy) {
		if (m_lightSources.count(entity) > 0) {
			m_lightSources.erase(entity);
		}
	}

}

void LightManager::addLight(Entity* _source)
{

	m_lightSources.insert(_source);

}


void LightManager::removeLight(Entity* _source)
{

	m_lightSources.erase(_source);

}

void LightManager::render(sf::RenderTarget& _surface)
{
	// Apply the camera view to the lightmap so that the lights follow
	m_lightMap.setView(_surface.getView());

	m_lightMap.clear(m_defaultColor);
	m_allLights.clear();


	for (Entity* entity : m_lightSources) {
		if (entity->isActive() == false || entity->getLightSource().value().isActive == false)
			continue;
		sf::Vector2f pos = entity->GetPosition();
		LightSource ls = entity->getLightSource().value();


		sf::Vertex centre = { pos, ls.color };

		sf::Vertex v;
		v.position = sf::Vector2f(centre.position.x + ls.radius * cos(0), centre.position.y + ls.radius * sin(0));
		v.color = ls.color;
		v.color.a = 0;

		sf::Vertex last = v;
		
		for (size_t i = 1; i < 16; i++) {

			m_allLights.append(centre);
			m_allLights.append(v);
			
			float angle = 2 * kPi / 16 * i;
			v.position = sf::Vector2f(centre.position.x + ls.radius * cos(angle), centre.position.y + ls.radius * sin(angle));
			v.color = ls.color;
			v.color.a = 0;
			m_allLights.append(v);
			
		}

		m_allLights.append(centre);
		m_allLights.append(v);
		m_allLights.append(last);

	}

	m_lightMap.draw(m_allLights);

	// fixed lights
	if (!m_uiLights.empty())
	{
		m_lightMap.setView(m_lightMap.getDefaultView());
		sf::VertexArray uiLights(sf::Triangles);

		for (auto& [pos, ls] : m_uiLights)
		{
			sf::Vertex centre = { pos, ls.color };
			sf::Vertex v;
			v.position = sf::Vector2f(pos.x + ls.radius, pos.y);
			v.color = ls.color;
			v.color.a = 0;
			sf::Vertex last = v;

			for (size_t i = 1; i < 16; i++)
			{
				uiLights.append(centre);
				uiLights.append(v);
				float angle = 2 * kPi / 16 * i;
				v.position = sf::Vector2f(pos.x + ls.radius * cos(angle), pos.y + ls.radius * sin(angle));
				v.color = ls.color;
				v.color.a = 0;
				uiLights.append(v);
			}
			uiLights.append(centre);
			uiLights.append(v);
			uiLights.append(last);
		}
		m_lightMap.draw(uiLights);
	}

	m_lightMap.display();
	sf::Sprite sprite(m_lightMap.getTexture());

	// Draw the sprite in screen coordinates (default view) to cover the entire screen
	sf::View savedView = _surface.getView();
	_surface.setView(_surface.getDefaultView());

	sf::RenderStates states;
	states.blendMode = sf::BlendMultiply;
	_surface.draw(sprite, states);

	_surface.setView(savedView);

}
