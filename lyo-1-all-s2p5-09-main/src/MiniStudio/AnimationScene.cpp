#include "AnimationScene.h"
#include "Wall.h"
#include "Player.h"



#include "AssetsManager.h"
#include "LightManager.h"
#include <random>
#include "MusicEnigme.h"
#include "POPEntity.h"
#include "TransitionManager.h"
#include "ParallaxManager.h"
#include "Debug.h"
#include "FirstScene.h"
#include "LightManager.h"
#include "ParallaxManager.h"
#include "Player.h"
#include "POPEntity.h"
#include "Stele.h"
#include "MusicEnigme.h"


void AnimationScene::spawnB(int x, int y)
{
	POPEntity* b = CreateEntity<POPEntity>(8, sf::Color::Red);
	b->SetTexture(AssetsManager::getInstance().getTexture("BKey", "res/Ui/BButton.png"));
	b->GetSprite().setScale({ 0.15f,0.15f });
	b->SetRigidBody(false);
	b->SetAffectedByGravity(false);
	b->SetAffectedByFriction(false);

	sf::Vector2f vel = sf::Vector2f((float)(rand() % 20 - 10), (float)(rand() % 20 - 20));
	b->AddVelocity(vel);

	LightSource ls;
	ls.color = sf::Color::Red;
	ls.radius = 35;
	ls.offset = sf::Vector2f();
	ls.isActive = true;

	b->init(2.f, ls);

	b->SetPosition(x, y,0,0);
	b->SetTag((int)("B"));



}

void AnimationScene::KillAllB()
{
	for (Entity* e : GameManager::Get()->getEntities()) {
		if (e->IsTag((int)("B"))) {
			e->Destroy();
		}
	}
}

void AnimationScene::BKeyBlink()
{
	m_blinkTimer += GetDeltaTime();


	if (m_blinkTimer >= m_blinkCD && !B->isActive())
	{
		B->setActive(true);
		m_blinkTimer = 0;
	}
	else if (m_blinkTimer >= m_blinkCD && B->isActive())
	{
	
		B->setActive(false);
		m_blinkTimer = 0;
	}


}

void AnimationScene::lunchWaitingScreen()
{
	TransitionManager::getInstance().SetCallBack(nullptr);

	m_loading = true;
	m_player->SetPosition(0, 0);



}

void AnimationScene::OnInitialize()
{
	B = CreateEntity<Entity>(35, sf::Color::Red);
	B->SetTexture(AssetsManager::getInstance().getTexture("BButton", "res/Ui/BButton.png"));
	B->GetSprite().setScale({ 0.2f,0.2f });
	B->SetRigidBody(false);
	B->SetAffectedByGravity(false);
	B->SetAffectedByFriction(false);
	B->SetPosition(360, 500, 1.5f, 1.5f);

	LightSource ls;
	ls.color = sf::Color::Red;
	ls.radius = 45;
	ls.offset = sf::Vector2f(1,1);
	ls.isActive = true;

	B->setLightSource(ls);

	m_audioMap->addSound("Ambiance", { "res/Audio/Sounds/Ambient/AmbianceSound.wav" });



	srand(time(NULL));
	m_map.load("res/maps/Cine.tmx");

	ParallaxManager::getInstance().init("res/Background");
	

	m_player = CreateEntity<Player>({ 12, 36 }, sf::Color::White);
	m_player->init(6);


	m_player->SetRight(Player::Action::CACatk, false);
	m_player->SetRight(Player::Action::Dash, false);
	m_player->SetRight(Player::Action::Datk, true);
	m_player->SetRight(Player::Action::Jump, true);
	m_player->SetRight(Player::Action::FastFalling, false);
	m_player->SetRight(Player::Action::Move, true);
	m_player->SetRight(Player::Action::Playing, false);
	m_player->SetRight(Player::Action::Teleport, false);


	
	m_player->SetRigidBody(true);
	m_player->setSpeed(20);
	m_player->SetTexture(AssetsManager::getInstance().getTexture("Player", "res/Player/test.png"));

	m_player->SetSpriteOffset({ -35.f, -35.f });
	m_player->SetSpriteOffsetFlipped({ -55, -35.f });

	m_player->GetAnimator().applyScale(m_player->GetSprite(), { 103, 80 });
	m_player->GetAnimator().loadFromJson("res/JsonAnimation/player.json");

	m_actions.bind(Action::BreakStone, { {JoystickButton::Est, TriggerState::Down} });
	m_actions.bind(Action::BreakStone, { {sf::Keyboard::B, TriggerState::Down} });
	m_actions.bind(Action::PlayEnigme, { {sf::Keyboard::P, TriggerState::Down} });

	for (const TmxObjectGroup& group : m_map.objectGroup)
	{
		if (group.name == "Collision")
		{
			for (const TmxObject& obj : group.objects)
			{
				Wall* wall = CreateEntity<Wall>(sf::Vector2f(obj.width, obj.height), sf::Color::Transparent);
				wall->SetPosition(obj.x, obj.y, 0.f, 0.f);
				wall->SetRigidBody(true);
				wall->SetStatic(true);
			}
		}
		if (group.name == "Plateform")
		{
			for (const TmxObject& obj : group.objects)
			{
				Wall* wall = CreateEntity<Wall>(sf::Vector2f(obj.width, obj.height), sf::Color::Transparent);
				wall->SetPlatform(true);
				wall->SetPosition(obj.x, obj.y, 0.f, 0.f);
				wall->SetRigidBody(true);
				wall->SetStatic(true);
			}
		}

		if (group.name == "Light") {
			for (const TmxObject& obj : group.objects)
			{
				Entity* light = CreateEntity<Entity>(1, sf::Color::Transparent);
				light->SetPosition(obj.x, obj.y, 0.f, 0.f);
				light->SetStatic(true);
				light->SetRigidBody(false);
				LightSource ls;
				ls.radius = std::get<float>(obj.proprieties.at("Radius"));
				ls.color = std::get<sf::Color>(obj.proprieties.at("Color"));
				ls.offset = { 0, 0 };
				ls.isActive = true;
				light->setLightSource(ls);
			}
		}

		if (group.name == "Custom") {
			for (const TmxObject& obj : group.objects) {
				if (obj.name == "PlayerSpawn") {
					m_player->SetPosition(obj.x, obj.y);
					m_player->setSpawnPoint({ obj.x, obj.y });
				}

				if (obj.name == "Stele") {
					Stele* tmp = CreateEntity<Stele>({ 75, 260 }, sf::Color::Transparent);
					tmp->SetPosition(obj.x, obj.y);
					tmp->setText(std::get<std::string>(obj.proprieties.at("Story")));

				}

			}
		}
	}

	m_cam->setPosition(m_player->GetPosition());
	GameManager::Get()->setCamera(*m_cam);

	ParallaxManager::getInstance().setCamera(m_cam);

	m_rock = CreateEntity<Entity>(75, sf::Color::Red);
	m_rock->SetPosition(330, 587);
	m_rock->SetTexture(AssetsManager::getInstance().getTexture("BrokenStone", "res/Items/StoneSprite.png"));
	m_rock->GetSprite().setTextureRect(sf::IntRect(sf::Vector2i(40, 25), sf::Vector2i(262, 262)));
	m_rock->GetSprite().setScale(sf::Vector2f(0.3, 0.3));


	m_loadingScreenSprite.setTexture(*AssetsManager::getInstance().getTexture("LoadingScreen", "res/Ui/LoadingScreen.png")->getTexture());


	m_onZoom = false;
	m_zoomTimer = 0.f;
	m_cam->zoom(0.7);


	m_player->setActive(false);

	SetGravity({ 0, 6 });

	TransitionManager::getInstance().SetFadeOut(true);
}

void AnimationScene::OnUpdate()
{
	m_audioMap->playSound("Ambiance");
	if (m_loading) {

		if (m_nbPoint > 100) {
			GameManager::Get()->LaunchScene<FirstScene>();
		}
		std::string txt;
		switch (m_nbPoint % 3)
		{
		case(1):
			txt = ".";
			break;

		case(2):
			txt = "..";
			break;

		case(0):
			txt = "...";
			break;
		default:
			txt = "";
			break;
		}

		Debug::DrawText(1200, 140, txt, sf::Color::White);
		m_nbPoint++;

		return;
	}

	if (m_player->GetPosition(0, 0).x > m_map.width * m_map.tileWidth && TransitionManager::getInstance().IsFadingIn() == false) {

		TransitionManager::getInstance().SetFadeIn(true);
		TransitionManager::getInstance().SetCallBack([this]() { lunchWaitingScreen(); });

		m_player->SetRight(Player::Action::Move, false);
		m_player->SetRigidBody(false);
		m_player->SetVelocityY(0);
		m_player->SetAffectedByGravity(false);

	}



	if (m_actions.isActive(Action::BreakStone) && m_stonePV > 0) {
	
		m_stonePV--;
		m_frameCount++;
		m_rock->GetSprite().setTextureRect(sf::IntRect(sf::Vector2i(247 * m_frameCount, 0), sf::Vector2i(m_rockTileWidth, m_rockTileHeight)));
		if (m_stonePV > 2)
			m_cam->zoom(0.9);

		if (m_stonePV <= 0) { 
			m_player->setActive(true);
			m_player->SetRight(Player::Action::Jump, true);
			m_player->SetRight(Player::Action::Move, true);
			B->Destroy();
			m_rock->GetSprite().setTextureRect(sf::IntRect(sf::Vector2i(247 * 9, 0), sf::Vector2i(m_rockTileWidth, m_rockTileHeight)));
			m_rock->SetPosition(325, 594);
		}
		
		m_timeSinceLastHit = 0;
	}
	
	else {
		m_timeSinceLastHit += GameManager::Get()->GetDeltaTime();
	}
	
	

	m_cam->setPosition(m_player->GetPosition());

	m_cam->CorrectPosition(sf::Vector2f(m_map.width * m_map.tileWidth, m_map.height * m_map.tileHeight));
	GameManager::Get()->setCamera(*m_cam);


	if (m_onZoom)
	{
		m_cam->zoom(m_zoomFactor);

		m_zoomTimer += GameManager::Get()->GetDeltaTime();

		if (m_zoomTimer >= m_zoomSpeed)
		{
			m_zoomSpeed = 0;
			m_zoomTimer = 0;
			m_onZoom = false;
		}
	}

	BKeyBlink();
}

void AnimationScene::OnDraw(sf::RenderTarget& target)
{
	target.draw(m_map);

	if (m_loading)
	{
		sf::RenderWindow* window = dynamic_cast<sf::RenderWindow*>(&target);
		if (window)
		{
			sf::View savedView = window->getView();
			window->setView(window->getDefaultView());
			m_loadingScreenSprite.setOrigin(0, 0);
			m_loadingScreenSprite.setScale(1, 1);
			m_loadingScreenSprite.setPosition(0, 0);
			window->draw(m_loadingScreenSprite);
			window->setView(savedView);
		}
	}
}

void AnimationScene::OnWindowEvent(const sf::Event& event)
{
}
