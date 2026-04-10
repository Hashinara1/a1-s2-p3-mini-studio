#pragma once

#include "LightSource.h"

#include <unordered_set>
#include <vector>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/VertexArray.hpp>


class Entity;

class LightManager
{
private:

	//light follow cam
	std::unordered_set<Entity*> m_lightSources;

	//light fix
	std::vector<std::pair<sf::Vector2f, LightSource>> m_uiLights;

	//where light
	sf::VertexArray m_allLights;

	//the light filter
	sf::RenderTexture m_lightMap;
	
	sf::Color m_defaultColor = sf::Color(50, 50, 50);

	LightManager() {

		m_lightMap.create(1280, 720); 
		m_allLights = sf::VertexArray(sf::Triangles);
	}


public:

	static LightManager& getInstance() {

		static LightManager instance = LightManager();
		return instance;
	}

	void update();

	void setDefaultColor(sf::Color _newColor) { m_defaultColor = _newColor; }

	void addLight(Entity* _source);
	void removeLight(Entity* _source);

	void addUiLight(sf::Vector2f _screenPos, LightSource _light) { m_uiLights.push_back({ _screenPos, _light }); }
	void clearUiLights() { m_uiLights.clear(); }
	void clearEntityLights() { m_lightSources.clear(); }

	void render(sf::RenderTarget& _surface);



};

