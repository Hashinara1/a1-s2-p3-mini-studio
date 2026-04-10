#pragma once

#include "Camera.h"

#include <list>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

class Entity;
class UI;
class Scene;
class Debug;
class QuadTree;

constexpr float kFixedDt = 1.f / 60.f;

namespace sf 
{
	class RenderWindow;
	class Event;
}

class GameManager
{
	std::list<UI*> mUIs;
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	sf::RenderWindow* mpWindow;
	sf::Font mFont;

	Scene* mpScene;
	Scene* mpNextScene;

	float mDeltaTime;
	float mAccumulatedDeltaTime;

	int mWindowWidth;
	int mWindowHeight;

	sf::Color mClearColor;

	QuadTree* mQuadTree;

	bool pause;
private:

	GameManager();

	void Run();
	
	void Update();
	void FixedUpdate();
	void Draw();

	


public:

	~GameManager();
	static GameManager* Get();

	void CreateWindow(unsigned int width, unsigned int height, const char* title, const char* _iconPath = nullptr, int fpsLimit = 60, sf::Color clearColor = sf::Color::Black);

	template<typename T>
	void LaunchScene();
	void addUI(UI* add);

	void SetUp();
	void setCamera(const Camera& cam);
	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }


	const std::list<Entity*>& getEntities() { return mEntities; }
	float GetDeltaTime() const { return mDeltaTime; }
	Scene* GetScene() const { return mpScene; }

	sf::Font& GetFont() { return mFont; };
	sf::RenderWindow* GetWindow() const { return mpWindow; }
	bool isPause() { return pause; }
	void SetPause(bool it) { pause = it; }

private:
	friend Debug;
	friend Scene;
	friend UI;
	friend class InputManager;
	friend class LightManager;
	friend class Entity;
	friend class TeleportPlayerAction;
	friend class QuadTree;
};

#include "GameManager.inl"