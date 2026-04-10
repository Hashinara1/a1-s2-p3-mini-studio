#pragma once

#include <vector>

#include <optional>
#include "Camera.h"

#include <SFML/Graphics.hpp>


class Entity;

class ParallaxManager
{
private:

	struct Layer
	{
		sf::Texture texture;
		sf::VertexArray quad;
		float speedFactor;
	};

	std::vector<Layer> m_bgLayers;
	std::vector<Layer> m_fgLayers;

	ParallaxManager() = default;

	ParallaxManager(const ParallaxManager&) = delete;
	ParallaxManager& operator=(const ParallaxManager&) = delete;

	std::shared_ptr<Camera> m_cam;
	std::optional<float> m_bgCamStartX;
	std::optional<float> m_fgCamStartX;

	void drawAny(sf::RenderTarget& _target, std::optional<float>& _startCam, std::vector<Layer>& _vector);

public:

	static ParallaxManager& getInstance() 
	{ 
		static ParallaxManager instance = ParallaxManager();
		return instance;
	}

	void init(const char* _path);
	void update();

	void drawBack(sf::RenderTarget& _target);
	void drawFront(sf::RenderTarget& _target);

	void setCamera(std::shared_ptr<Camera> _cam) { m_cam = _cam; }

};

