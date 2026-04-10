#include "GameManager.h"
#include "ParallaxManager.h"
#include "Entity.h"
#include "Debug.h"
#include "DialogueManager.h"
#include "InputManager.h"
#include "LightManager.h"
#include "QuadTree.h"
#include "TransitionManager.h"

#include <algorithm>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include "QuadTree.h"
#include "UI.h"

GameManager::GameManager()
{
	mQuadTree = nullptr;
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mpNextScene = nullptr;
	mAccumulatedDeltaTime = 0;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, const char* _iconPath, int fpsLimit, sf::Color clearColor)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
	mpWindow->setFramerateLimit(fpsLimit);

	if (_iconPath != nullptr);
	{
		sf::Image icon;
		icon.loadFromFile(_iconPath);

		mpWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
}
void GameManager::addUI(UI* add)
{
	add->OnInit();
	mUIs.push_back(add);
}

void GameManager::SetUp()
{
	mpScene = mpNextScene;
	mpNextScene = nullptr;

	mpScene->SetGameManager(this);
	mpScene->OnInitialize();

	Run();
}

float timer = 0.0f;
int FPS = 0;

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("../../../res/Hack-Regular.ttf");
	_ASSERT(fontLoaded);

	_ASSERT(mpScene != nullptr);

	sf::Clock clock;
	InputManager& inputManager = InputManager::getInstance();

	DialogueManager& dialogueManager = DialogueManager::getInstance();
	LightManager& lightManager = LightManager::getInstance();
	TransitionManager& transitionManager = TransitionManager::getInstance();

	ParallaxManager& parallaxManager = ParallaxManager::getInstance();

	while (mpWindow->isOpen())
	{

		//We cap the dt at 0.05 so that if there's a big lag, it doesn't generate too many frames
		SetDeltaTime(std::min(clock.restart().asSeconds(), 0.05f));


		timer += mDeltaTime;
		if (timer > 1.0f) {
			FPS = (int)(1.0 / mDeltaTime);
			timer = 0.0f;
		}

		
		Debug::DrawText(mpWindow->getView().getCenter().x + 900, mpWindow->getView().getCenter().y - 800, "FPS: " + std::to_string(FPS), sf::Color::White);




		inputManager.update();

		dialogueManager.update();

		transitionManager.update(GetDeltaTime(), mpWindow->getView().getCenter());

		parallaxManager.update();

		Update();

		if (mpNextScene != nullptr)
		{
			//	delete all entity from the previous scene
			for (Entity* e : mEntities) delete e;
			for (Entity* e : mEntitiesToAdd) delete e;
			for (Entity* e : mEntitiesToDestroy) delete e;
			mEntities.clear();
			mEntitiesToAdd.clear();
			mEntitiesToDestroy.clear();

			lightManager.clearUiLights();
			lightManager.clearEntityLights();

			//	take the cam to use it in the next scene
			std::shared_ptr<Camera> cam = mpScene->getCam();

			//	change current scene
			delete mpScene;
			mpScene = mpNextScene;

			mpScene->setCam(cam);

			mpNextScene = nullptr;
			mpScene->SetGameManager(this);
			mpScene->OnInitialize();
		}

		Draw();

		
	}


}

void GameManager::setCamera(const Camera& cam)
{
	mpWindow->setView(cam.getView());
}

void GameManager::Update()
{


	mpScene->OnUpdate();

	mAccumulatedDeltaTime += mDeltaTime;
	while (mAccumulatedDeltaTime > kFixedDt)
	{
		// gravity
		for (Entity* entity : mEntities)
			entity->ApplyGravity();

		//shift
		FixedUpdate();

		if (mQuadTree)
			delete mQuadTree;
		//(float)mpScene->m_map.width* mpScene->m_map.tileWidth, (float)mpScene->m_map.height* mpScene->m_map.tileHeight
		mQuadTree = new QuadTree(Rect({ 0,0 }, { (float)mpScene->m_map.width * mpScene->m_map.tileWidth, (float)mpScene->m_map.height * mpScene->m_map.tileHeight }));

		//colision	
		mQuadTree->init(mEntities);
		mQuadTree->UpdateCollision();


		mAccumulatedDeltaTime -= kFixedDt;
	}

	LightManager::getInstance().update();

	// delete entity
	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}



	mEntitiesToDestroy.clear();

	// add entity
	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::FixedUpdate()
{
	if (!pause) {
		for (auto it = mEntities.begin(); it != mEntities.end(); )
		{
			Entity* entity = *it;
			if (entity->isActive() == false) {
				it++;
				continue;
			}
			entity->FixedUpdate();
			if (entity->ToDestroy() == false)
			{
				++it;
				continue;
			}
			mEntitiesToDestroy.push_back(entity);
			it = mEntities.erase(it);
		}
	}
	for (UI* it : mUIs)
	{
		if (it->isActif())
			it->FixedUpdate();
	}
}

void GameManager::Draw()
{
	mpWindow->clear(mClearColor);

	sf::View cameraView = mpWindow->getView();
	sf::View defaultView = mpWindow->getDefaultView();

	mpWindow->setView(defaultView);

	ParallaxManager::getInstance().drawBack(*mpWindow);

	mpWindow->setView(cameraView);

	mpScene->OnDraw(*mpWindow);


	for (Entity* entity : mEntities)
	{
		if (entity->isActive() == false)
			continue;
		if (entity->HasTexture())
			mpWindow->draw(entity->mSprite);
		else
			mpWindow->draw(entity->GetShape());

		
	}
	for (UI* it : mUIs)
	{
		if (it->isActif())
			it->Render();
	}

	mpScene->DrawUI(*mpWindow);

	LightManager::getInstance().render(*mpWindow);

	Debug::Get()->Draw(mpWindow);

	cameraView = mpWindow->getView();
	defaultView = mpWindow->getDefaultView();

	mpWindow->setView(defaultView);

	ParallaxManager::getInstance().drawFront(*mpWindow);

	DialogueManager::getInstance().Draw(*mpWindow);


	mpWindow->setView(cameraView);

	mpWindow->display();
}
