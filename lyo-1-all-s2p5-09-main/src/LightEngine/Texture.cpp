#include "Texture.h"

#include "SFML/Graphics.hpp"

Texture::Texture(const char* _path)
{
    m_texture = new sf::Texture;
    if (!m_texture->loadFromFile(_path)) {
        delete m_texture;
        m_texture = nullptr;
        std::abort(); // or exit(1)
    }

    sf::Vector2u sizes = m_texture->getSize();
    m_width = sizes.x;
    m_height = sizes.y;
}
Texture::~Texture()
{
    delete m_texture;
};