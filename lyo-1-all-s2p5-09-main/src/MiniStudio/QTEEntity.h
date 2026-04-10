#pragma once
#include "Entity.h"
#include "LightManager.h"
#include "GameManager.h"
#include <cmath>

class QTEEntity : public Entity
{
private:
	float m_timer = 0.f;
	static constexpr float kPulseSpeed = 3.f;

public:
	void OnInitialize() override
	{
		SetTexture(AssetsManager::getInstance().getTexture("ButtonA", "res/Ui/AButton.png"));
		mSprite.setScale(3, 3);
		SetSpriteOffset(sf::Vector2f(-340, -340));
		LightSource ls;
		ls.color = sf::Color(0, 255, 0, 255);
		ls.radius = 800.f;
		ls.isActive = true;
		setLightSource(ls);
		LightManager::getInstance().addLight(this);
	}

	void OnFixedUpdate() override
	{
		m_timer += GameManager::Get()->GetDeltaTime();
		float alpha = (std::sin(m_timer * kPulseSpeed) + 1.f) / 2.f * 255.f;

		if (mLight.has_value())
		{
			mLight->color.a = static_cast<uint8_t>(alpha);
		}
	}
};
