#pragma once
#include "ActionMap.h"
#include "AudioMap.h"
#include "Scene.h"

#include "TmxThing.h"

#include <vector>

class Player;
class MusicEnigme;

class AnimationScene : public Scene
{

	enum class Action {
		BreakStone,
		PlayEnigme
	};

private:
	Entity* B;

	TmxMap m_map;
	AudioMap* m_audioMap = new AudioMap();
	ActionMap<Action> m_actions;

	sf::Sprite m_loadingScreenSprite;

	Entity* m_rock;

	int m_nbPoint = 1;

	int m_stonePV = 9;
	Player* m_player;

	bool m_loading = false;

	bool m_onZoom;

	float m_zoomFactor;
	float m_zoomSpeed;

	float m_zoomTimer;
	float m_blinkCD = 0.75f;
	float m_timeSinceLastHit = 0;
	float m_blinkTimer = 0;
	int m_rockTileWidth = 261;
	int m_rockTileHeight = 262;
	int m_frameCount = 0;


	void spawnB(int x, int y);
	void KillAllB();
	void BKeyBlink();
	void lunchWaitingScreen();
public:
	MusicEnigme* m_enigme;

	void OnDraw(sf::RenderTarget& target) override;
	void OnInitialize() override;
	void OnUpdate() override;
	void OnWindowEvent(const sf::Event& event) override;
};

