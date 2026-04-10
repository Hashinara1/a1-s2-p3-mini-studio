#pragma once

#include <vector>

#include "Camera.h"
#include "Player.h"
#include "Scene.h"
#include "TmxThing.h"
#include "Wall.h"


class MusicEnigme;
class FirstScene : public Scene
{
private:
	Player* m_player;

	//UI
	sf::Sprite m_firstWeapon;
	sf::Sprite m_healthBar;
	sf::Sprite m_heart;
	sf::Sprite m_mana;
	sf::Sprite m_WeaponSlot;

	std::vector<sf::Vector2f> m_heartPos;
	std::vector<sf::Vector2f> m_manaPos;
	std::vector<sf::Vector2i> m_manaRect;
	std::vector<sf::Vector2f> m_weaponPos;
	std::vector<sf::Sprite> m_weapons;

	bool m_onZoom;

	float m_zoomFactor;
	float m_zoomSpeed;

	float m_zoomTimer;

	float m_FadeSpeedIn;
	float m_FadeSpeedOut;

	float m_alphaOut;
	float m_alphaIn;

	float m_radiusIn;
	float m_radiusOut;

	TmxMap m_map;


	void LoadSound();

public:
	MusicEnigme* enigme1;

	void OnDraw(sf::RenderTarget& target) override;
	void OnInitialize() override;
	void OnUpdate() override;
	void OnWindowEvent(const sf::Event& event) override;
	Player* getPlayer() { return m_player; }
	void zoom(float _factor, float _zoomSpeed);


	void FadeTransitionIn(float speed);
	void FadeTransitionOut(float speed);


	friend class Lizard;
};

