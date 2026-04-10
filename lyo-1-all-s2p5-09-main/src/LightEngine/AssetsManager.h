#pragma once

#include "Texture.h"

#include <memory>
#include <string>
#include <unordered_map>


class AssetsManager
{

private:

	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

	AssetsManager() = default;

	AssetsManager(const AssetsManager&) = delete;
	AssetsManager& operator=(const AssetsManager&) = delete;

public:

	static AssetsManager& getInstance() {
		static AssetsManager instance;
		return instance;
	}

	std::shared_ptr<Texture> getTexture(std::string _name);
	std::shared_ptr<Texture> getTexture(std::string _name, const char* _path);



	~AssetsManager() {
		m_textures.clear();
	}
};