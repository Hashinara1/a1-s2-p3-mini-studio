#pragma once
#include "Scene.h"

#include "TmxThing.h"

class Player;

class LvL4Scene : public Scene
{
private:
	sf::Shader m_invertShader;
	sf::Sprite m_loadingScreenSprite;
	sf::Sprite m_deathScreenSprite;


	Player* m_player;

	//UI
	sf::Sprite m_healthBar;
	sf::Sprite m_heart;
	sf::Sprite m_mana;
	sf::Sprite m_WeaponSlot;
	sf::Sprite m_firstWeapon;
	std::vector<sf::Sprite> m_weapons;
	std::vector<sf::Vector2f> m_weaponPos;
	std::vector<sf::Vector2f> m_heartPos;
	std::vector<sf::Vector2f> m_manaPos;
	std::vector<sf::Vector2i> m_manaRect;

	UI* pauseUI;


	bool m_loading = false;

	bool m_onZoom;
	float m_zoomFactor;
	float m_zoomSpeed;
	float m_zoomTimer;
	float m_timer;

	void setUpUI();
	void ParseMap();
	void InitPlayer();
	void InitCam();

	void lunchWaitingScreen();

public:
	// Hérité via Scene
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;

	void DrawUI(sf::RenderTarget& target) override; 
	void OnDraw(sf::RenderTarget& target) override;
};

