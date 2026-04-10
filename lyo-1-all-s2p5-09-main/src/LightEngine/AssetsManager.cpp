#include "AssetsManager.h"


std::shared_ptr<Texture> AssetsManager::getTexture(std::string _name)
{

	if (m_textures.find(_name) == m_textures.end()) {
		return nullptr;
	}
	return m_textures[_name];
}

std::shared_ptr<Texture> AssetsManager::getTexture(std::string _name, const char* _path)
{
	if (m_textures.find(_name) != m_textures.end()) {
		return m_textures[_name];
	}
	std::string fullPath = "../../../";
	fullPath += _path;
	std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(new Texture(fullPath.c_str()));
	m_textures[_name] = texture;

	return m_textures[_name];
}
