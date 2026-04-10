#pragma once

#include "Camera.h"
#include "TmxThing.h"
#include "AudioMap.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class GameManager;

class MusicEnigme;

class UI;
class Scene
{
private:

	GameManager* mpGameManager;
	sf::Vector2f m_gravity = {0, 1};


	bool m_isFadingIn;
	bool m_isFadingOut;

private:

	void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	
protected:

	TmxMap m_map;
	std::shared_ptr<Camera> m_cam = std::make_shared<Camera>();

	AudioMap* m_audioMap = new AudioMap();

	Scene() = default;
	virtual ~Scene() = default;


	virtual void OnDraw(sf::RenderTarget& target) {}
	virtual void DrawUI(sf::RenderTarget& target) {}
	virtual void OnInitialize() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnWindowEvent(const sf::Event& event) = 0;

public:
	MusicEnigme* enigme1;

	template<typename T>
	T* CreateEntity(float radius, const sf::Color& color);

	template<typename T>
	T* CreateEntity(sf::Vector2f size, const sf::Color& color);

	float GetDeltaTime() const;

	AudioMap* getAudioMap() { return m_audioMap; }
	int GetWindowHeight() const;
	int GetWindowWidth() const;

    sf::Vector2f GetGravity() { return m_gravity; }
	void SetGravity(sf::Vector2f gravity) { m_gravity = gravity; }

	void setCam(std::shared_ptr<Camera> _cam) { m_cam = _cam; }
	std::shared_ptr<Camera> getCam() { return m_cam; }

	TmxMap* GetMap() { return &m_map; }

private:
	friend UI;

	friend GameManager;

	friend class InputManager;
};

#include "Scene.inl"