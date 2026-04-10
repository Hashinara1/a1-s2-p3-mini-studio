#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>

class TransitionManager
{
private :

	bool m_isFadingIn;
	bool m_isFadingOut;

	float m_alphaIn = 0;
	float m_FadeSpeed = 150;
	float m_radiusIn = 0;

	float m_alphaOut = 255;
	float m_radiusOut = 1280 * 1.25;

	std::function<void()> m_callBack;

public:

	static TransitionManager& getInstance() {
		static TransitionManager instance = TransitionManager();
		return instance;
	}

	void update(float dt, sf::Vector2f _center);

	void SetCallBack(std::function<void()> _func) { m_callBack = _func; }

	void SetFadeIn(bool set) { m_isFadingIn = set; }
	void SetFadeOut(bool set) { m_isFadingOut = set; }

	bool IsFadingOut() { return m_isFadingOut; }
	bool IsFadingIn() { return m_isFadingIn; }

};

