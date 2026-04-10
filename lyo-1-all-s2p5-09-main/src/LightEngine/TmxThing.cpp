#include "TmxParser.h"
#include "TmxThing.h"

#include <climits>
#include <iostream>

bool TmxMap::load(const std::string& path)
{
	std::string fullpath = "../../../";
	fullpath += path;
	auto result = TmxParser::load(fullpath);
	if (!result)
	{
		std::cerr << "failed to load: " << path << "\n";
		return false;
	}

	// copy the data
	*static_cast<TmxMap*>(this) = std::move(*result);

	// loads one texture per tileset
	for (const TmxTileset& ts : tileset)
	{
		sf::Texture tex;
		if (!tex.loadFromFile(ts.path))
		{
			std::cerr << "failed to load texture: " << ts.path << "\n";
			return false;
		}
		m_textures.push_back(std::move(tex));
	}

	// constructs a VertexArray by (layer x tileset)
	for (const TmxLayer& layer : layers)
	{
		for (int tsIdx = 0; tsIdx < (int)tileset.size(); tsIdx++)
		{
			const TmxTileset& ts = tileset[tsIdx];
			int nextFirstGid = (tsIdx + 1 < (int)tileset.size()) ? tileset[tsIdx + 1].firstGid : INT_MAX;

			RenderLayer rl;
			rl.vertices.setPrimitiveType(sf::Quads);
			rl.tilesetIndex = tsIdx;

			for (int y = 0; y < layer.height; y++)
			{
				for (int x = 0; x < layer.width; x++)
				{
					const TmxTile& tile = layer.tiles[y * layer.width + x];
					if (tile.gid == 0 || tile.gid < ts.firstGid || tile.gid >= nextFirstGid)
						continue;

					int localId = tile.gid - ts.firstGid;
					int col = localId % ts.columns;
					int row = localId / ts.columns;

					float px = (float)(x * tileWidth);
					float py = (float)(y * tileHeight);
					float pw = (float)tileWidth;
					float ph = (float)tileHeight;

					float u  = (float)(col * ts.tileWidth);
					float v  = (float)(row * ts.tileHeight);
					float uw = (float)ts.tileWidth;
					float uh = (float)ts.tileHeight;

					// UV corners : TL, TR, BR, BL
					sf::Vector2f uvs[4] = { {u, v}, {u+uw, v}, {u+uw, v+uh}, {u, v+uh} };

					if (tile.flipD) { std::swap(uvs[1], uvs[3]); }
					if (tile.flipH) { std::swap(uvs[0], uvs[1]); std::swap(uvs[3], uvs[2]); }
					if (tile.flipV) { std::swap(uvs[0], uvs[3]); std::swap(uvs[1], uvs[2]); }

					sf::Vertex v0, v1, v2, v3;
					v0.position = { px,      py      }; v0.texCoords = uvs[0];
					v1.position = { px + pw, py      }; v1.texCoords = uvs[1];
					v2.position = { px + pw, py + ph }; v2.texCoords = uvs[2];
					v3.position = { px,      py + ph }; v3.texCoords = uvs[3];

					rl.vertices.append(v0);
					rl.vertices.append(v1);
					rl.vertices.append(v2);
					rl.vertices.append(v3);
				}
			}

			if (rl.vertices.getVertexCount() > 0)
				m_renderLayers.push_back(std::move(rl));
		}
	}

	return true;
}

void TmxMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const RenderLayer& rl : m_renderLayers)
	{
		states.texture = &m_textures[rl.tilesetIndex];
		target.draw(rl.vertices, states);
	}
}