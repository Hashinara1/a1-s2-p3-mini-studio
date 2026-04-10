#include "Animator.h"

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "json.hpp"
using json = nlohmann::json;

#include <fstream>


void Animator::loadFromJson(const char* _path)
{
	std::string fullpath = "../../../";
	fullpath += _path;

	std::ifstream file(fullpath);
	json j = json::parse(file);


	m_animationSet.frameHeight = j["frame_size"]["height"];
	m_animationSet.frameWidth = j["frame_size"]["width"];

	for (auto& [name, anim] : j["animations"].items())
	{
		AnimationClip ac;

		ac.loop = anim["loop"].get<bool>();
		ac.frameTime = anim["frame_time"].get<float>();

		for (auto& frame : anim["frames"])
			ac.frames.push_back(frame.get<int>());
		m_animationSet.clips[name] = ac;
		if (m_currentAnimation == "")
			m_currentAnimation = name;
	}

	m_currentFrame = 0;





}

void Animator::addClip(const std::string& _name, std::vector<int> _frames, float _frameTime, bool _loop, int _frameWidth, int _frameHeight)
{
	AnimationClip clip;
	clip.frames = _frames;
	clip.frameTime = _frameTime;
	clip.loop = _loop;
	m_animationSet.clips[_name] = clip;
	m_animationSet.frameWidth = _frameWidth;
	m_animationSet.frameHeight = _frameHeight;
	if (m_currentAnimation.empty())
		m_currentAnimation = _name;
}

void Animator::play(std::string _name)
{
	if (m_animationSet.clips.count(_name) < 1)
		return;

	if (m_currentAnimation == _name) 
		return;

	m_currentAnimation = _name;
	m_currentFrame = 0;
}

void Animator::update(float _dt, sf::Sprite& _sprite)
{
	m_timer += _dt;
	
	const AnimationClip& animationclip = getCurrentClip();

	if (m_timer < animationclip.frameTime)
		return;

	m_currentFrame++;
	m_timer = 0.f;

	const auto& frames = animationclip.frames;
	if (m_currentFrame >= (int)frames.size())
	{
		if (animationclip.loop)
			m_currentFrame = 0;
		else
		{
			m_currentFrame = (int)frames.size() - 1;
			return;
		}
	}

	sf::Vector2u texSize = _sprite.getTexture()->getSize();
	int cols = texSize.x / m_animationSet.frameWidth;

	int col = frames[m_currentFrame] % cols;
	int row = frames[m_currentFrame] / cols;
	sf::IntRect rect(col * m_animationSet.frameWidth, row * m_animationSet.frameHeight, m_animationSet.frameWidth, m_animationSet.frameHeight);
	_sprite.setTextureRect(rect);

}

bool Animator::isFinished() const
{
	const AnimationClip& animationclip = getCurrentClip();
	if (animationclip.loop == true)
		return false;
	return m_currentFrame >= animationclip.frames.size() -1;
}

void Animator::applyScale(sf::Sprite& _sprite, sf::Vector2f _entitySize)
{
	_sprite.setScale(
		_entitySize.x / m_animationSet.frameWidth,
		_entitySize.y / m_animationSet.frameHeight
	);

	const AnimationClip& animationclip = getCurrentClip();
	const auto& frames = animationclip.frames;

	sf::Vector2u texSize = _sprite.getTexture()->getSize();
	int cols = texSize.x / m_animationSet.frameWidth;

	int col = frames[m_currentFrame] % cols;
	int row = frames[m_currentFrame] / cols;
	sf::IntRect rect(col * m_animationSet.frameWidth, row * m_animationSet.frameHeight, m_animationSet.frameWidth, m_animationSet.frameHeight);
	_sprite.setTextureRect(rect);
}


const AnimationClip& Animator::getCurrentClip() const
{
	if (auto it = m_animationSet.clips.find(m_currentAnimation); it != m_animationSet.clips.end())
	if (auto it = m_animationSet.clips.find(m_currentAnimation); it != m_animationSet.clips.end())
		return (*it).second;
	
}
