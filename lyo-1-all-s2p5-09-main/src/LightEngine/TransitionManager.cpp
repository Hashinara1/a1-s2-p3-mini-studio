#include "Debug.h"
#include "TransitionManager.h"

void TransitionManager::update(float dt,sf::Vector2f _center)
{
	if (m_isFadingIn)
	{
		m_alphaIn += m_FadeSpeed * dt / 1.5;
		m_radiusIn += m_FadeSpeed * dt * 2;

		if (m_alphaIn >= 255)
		{
			m_alphaIn = 0;
			m_isFadingIn = false;
			m_radiusIn = 0;
			if (m_callBack)
				m_callBack();
		}

		Debug::DrawCircle(_center.x, _center.y, m_radiusIn, sf::Color(0, 0, 0, (int)m_alphaIn));
	}

	if (m_isFadingOut)
	{
		m_alphaOut -= m_FadeSpeed * dt / 2.25;
		m_radiusOut -= m_FadeSpeed * dt * 2;

		Debug::DrawCircle(_center.x, _center.y, m_radiusOut, sf::Color(0, 0, 0, (int)m_alphaOut));

		if (m_alphaOut <= 0)
		{
			m_alphaOut = 255;
			m_isFadingOut = false;
			m_radiusOut = 1280 * 1.25;
			if (m_callBack)
				m_callBack();
		}
	}
}



