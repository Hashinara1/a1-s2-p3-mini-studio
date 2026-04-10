#pragma once

#include <stdexcept>

#include <SFML/Graphics/Texture.hpp>

class Texture {

private:

	sf::Texture* m_texture = nullptr;

	float m_width = 0;
	float m_height = 0;

	Texture(const char* _path);

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

public:

	~Texture();

	sf::Texture* getTexture() const { return m_texture; }

	float getHeight() const { return m_height; }
	float getWidth() const { return m_width; }

private:

	friend class AssetsManager;
	friend class Renderer;
};
