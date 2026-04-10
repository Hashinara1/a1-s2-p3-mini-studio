#pragma once

#include "TmxThing.h"

#include <optional>


namespace tinyxml2 {

	class XMLElement;
};


class TmxParser {

public:

	static std::optional<TmxMap> load(const std::string& _path);

private:

	static sf::Color colorFromTmx(const std::string& _hex);

	static std::optional<TmxTileset> loadTileSet(const std::string& _path, tinyxml2::XMLElement* _elem);
	static std::optional<TmxLayer> loadLayer(tinyxml2::XMLElement* _elem);

	static TmxObject loadObject(tinyxml2::XMLElement* _elem);
	static TmxObjectGroup loadObjectGroup(tinyxml2::XMLElement* _elem);

};
