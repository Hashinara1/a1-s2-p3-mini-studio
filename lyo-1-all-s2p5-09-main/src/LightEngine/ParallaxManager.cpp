#include "ParallaxManager.h"
#include "AssetsManager.h"
#include "Entity.h"
#include "Texture.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


void ParallaxManager::drawAny(sf::RenderTarget& _target, std::optional<float>& _startCam, std::vector<Layer>& _vector)
{
    float width = (float)GameManager::Get()->GetScene()->GetWindowWidth();
    float height = (float)GameManager::Get()->GetScene()->GetWindowHeight();
    float camX = m_cam->getView().getCenter().x;
    float zoomLevel = height / m_cam->getView().getSize().y;
    if (zoomLevel < 1)
    {
        zoomLevel = 1;
    }

    if (!_startCam) _startCam = camX;

    for (auto& layer : _vector)
    {
        float texWidth = (float)layer.texture.getSize().x;
        float texHeight = (float)layer.texture.getSize().y;

        float moveScale = height / texHeight;
        float quadWidthBase = texWidth * moveScale;

        float scale = moveScale * zoomLevel;
        float quadWidth = texWidth * scale;
        float quadHeight = texHeight * scale;

        float layerOffset = -(camX - *_startCam) * layer.speedFactor;
        float layerOffset_px = layerOffset * moveScale;

        float centerX = width / 2.f;
        float offsetFromCenter = std::fmod(layerOffset_px, quadWidthBase);
        if (offsetFromCenter > 0.f) offsetFromCenter -= quadWidthBase;
        offsetFromCenter *= zoomLevel;

        float drawStartX = centerX + offsetFromCenter
            - std::fmod(centerX, quadWidthBase) * zoomLevel
            - quadWidth;

        int   numQuads = (int)std::ceil(width / quadWidth) + 3;
        float top = (height - quadHeight) / 2.f;

        for (int i = 0; i < numQuads; ++i)
        {
            float drawX = drawStartX + i * quadWidth;

            layer.quad[0].position = { drawX, top };
            layer.quad[1].position = { drawX, top + quadHeight };
            layer.quad[2].position = { drawX + quadWidth, top + quadHeight };
            layer.quad[3].position = { drawX + quadWidth, top };

            layer.quad[0].texCoords = { 0, 0 };
            layer.quad[1].texCoords = { 0, texHeight };
            layer.quad[2].texCoords = { texWidth, texHeight };
            layer.quad[3].texCoords = { texWidth, 0 };

            sf::RenderStates states;
            states.texture = &layer.texture;
            _target.draw(layer.quad, states);
        }
    }

}

void ParallaxManager::init(const char* _path)
{
    std::string fullPath = "../../../";
    fullPath += _path;

    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(fullPath))
    {
        files.push_back(entry.path());
    }

    std::sort(files.begin(), files.end());

    float factor = 0.f;

    for (const auto& filePath : files)
    {
        Layer layer;
        layer.texture.loadFromFile(filePath.string());
        layer.quad = sf::VertexArray(sf::Quads, 4);
        layer.speedFactor = factor;
        factor += 0.03f;
        std::string path = (std::string)_path;
        path = path.substr(0, 14);


        if (path == "res/Background")
        {
            m_bgLayers.push_back(layer);
            m_bgCamStartX.reset();
        }
        else
        {
            m_fgLayers.push_back(layer);
            m_fgCamStartX.reset();
        }
    }
}

void ParallaxManager::update()
{

}


void ParallaxManager::drawBack(sf::RenderTarget& target)
{
    drawAny(target, m_bgCamStartX, m_bgLayers);
}

void ParallaxManager::drawFront(sf::RenderTarget& target)
{
    drawAny(target, m_fgCamStartX, m_fgLayers);

}