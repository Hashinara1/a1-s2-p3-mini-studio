#pragma once

#include "SFML/System/Vector2.hpp"

#include <vector>
#include <string>
#include <unordered_map>


namespace sf {

	class Sprite;
}


struct AnimationClip {

	std::vector<int> frames;
	float frameTime;
	bool loop;
};


struct AnimationSet {

	std::unordered_map<std::string, AnimationClip> clips;
	int frameWidth;
	int frameHeight;
};


class Animator
{ 
private:

	AnimationSet m_animationSet;

	std::string m_currentAnimation;

	int m_currentFrame;

	float m_timer;

public:

	void addClip(const std::string& _name, std::vector<int> _frames, float _frameTime, bool _loop, int _frameWidth, int _frameHeight);
	void play(std::string _name);
	void applyScale(sf::Sprite& _sprite, sf::Vector2f _entitySize);
	bool isFinished() const ;
	void loadFromJson(const char* _path);
	void update(float _dt, sf::Sprite& _sprite);

	const AnimationClip& getCurrentClip()const;


};

