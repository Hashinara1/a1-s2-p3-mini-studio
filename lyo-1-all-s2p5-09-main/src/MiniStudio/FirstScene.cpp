#include "AudioManager.h"
#include "AssetsManager.h"
#include "Debug.h"
#include "Define.h"
#include "FirstScene.h"
#include "InputManager.h"
#include "LightManager.h"
#include "ParallaxManager.h"
#include "ShadowOfStorm.h"
#include "Stele.h"
#include "TmxThing.h"
#include "TransitionManager.h"

#include <variant>
#include <iostream>

void FirstScene::LoadSound()
{
	AudioManager::getInstance().load("res/Audio/Sounds/Elemental/Explosion.wav");
}

void FirstScene::OnInitialize()
{
	LoadSound();


	m_player = CreateEntity<Player>({ 18, 36 }, sf::Color::White);
	m_player->init(6);


	m_player->SetRight(Player::Action::Jump, true);
	m_player->SetRight(Player::Action::Move, true);
	m_player->SetRight(Player::Action::FastFalling, true);
	m_player->SetRight(Player::Action::Dash, true);
	m_player->SetRight(Player::Action::CACatk, true);
	m_player->SetRight(Player::Action::Datk, true);
	m_player->SetRight(Player::Action::Playing, true);
	m_player->SetRight(Player::Action::Teleport, true);


	m_player->SetTexture(AssetsManager::getInstance().getTexture("Player", "res/Player/test.png"));
	m_player->SetRigidBody(true);
	m_player->setSpeed(20);

	m_player->GetAnimator().loadFromJson("res/JsonAnimation/player.json");
	m_player->SetSpriteOffset({ -35.f, -35.f });
	m_player->SetSpriteOffsetFlipped({ -55, -35.f });

	m_player->GetAnimator().applyScale(m_player->GetSprite(), {103, 80});




	m_map.load("res/maps/LvL1.tmx");

	ParallaxManager::getInstance().init("res/Background");
	

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
				wall->SetPosition(obj.x, obj.y, 0.f, 0.f);
				wall->SetRigidBody(true);
				wall->SetStatic(true);
				wall->SetPlatform(true);
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
				}

				if (obj.name == "Stele") {
					Stele* tmp = CreateEntity<Stele>({ 700, 1500 }, sf::Color::Red);
					tmp->SetPosition(obj.x, obj.y);
					tmp->setText(std::get<std::string>(obj.proprieties.at("Story")));
				}

				if (obj.name == "SOSspawn") {
					ShadowOfStorm* sos = CreateEntity<ShadowOfStorm>({ 30, 60 }, sf::Color::Red);
					sos->SetPosition(obj.x, obj.y);
					sos->init(1500);
				}

			}
		}
	}

	m_onZoom = false;
	m_zoomTimer = 0.f;

	SetGravity({ 0, 6 });

	m_healthBar.setPosition(sf::Vector2f(0, 0));
	m_healthBar.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_healthBar.setTextureRect(sf::IntRect({ 0, 0 }, { 1175, 600 }));
	m_healthBar.setScale(sf::Vector2f(0.5, 0.5));

	m_heart.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_heart.setTextureRect(sf::IntRect({ 1796, 196 }, { 100, 100 }));
	m_heart.setScale(sf::Vector2f(0.5, 0.5));

	m_mana.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_mana.setScale(0.5, 0.5);

	//m_WeaponSlot.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	//m_WeaponSlot.setTextureRect(sf::IntRect({ 0, 1600 }, { 700, 700 }));
	//m_WeaponSlot.setScale(0.5, 0.5);
	//m_WeaponSlot.setPosition(sf::Vector2f(0, 350));


	sf::Sprite sprite;
	sprite.setTexture(*AssetsManager::getInstance().getTexture("Guitar", "res/Items/guitar.png")->getTexture());
	sprite.setScale(0.1, 0.1);
	//m_weapons.push_back(sprite);


	sprite.setTexture(*AssetsManager::getInstance().getTexture("Lyre", "res/Items/lyre.png")->getTexture());
	sprite.setScale(0.08, 0.08);
	//m_weapons.push_back(sprite);

	sprite.setTexture(*AssetsManager::getInstance().getTexture("Tambourin", "res/Items/tambourin.png")->getTexture());
	sprite.setScale(0.08, 0.08);
	//m_weapons.push_back(sprite);


	//m_weaponPos = { {90, 385}, {65, 565}, {225, 565} };
	m_heartPos = { {51, 57}, {128, 55}, {206, 56}, {290, 55}, {370, 55}, {450, 57} };
	m_manaRect = { {2515, 2000}, {2545, 1560} , {2545, 1135}, {2515, 675} , {2555, 175} };
	m_manaPos = { {80, 90}, {95, 96}, {98, 102}, {110, 110}, {131, 110} };

	m_cam->setPosition(m_player->GetPosition());
	GameManager::Get()->setCamera(*m_cam);
	
	ParallaxManager::getInstance().setCamera(m_cam);

	LightManager::getInstance().setDefaultColor(sf::Color(50, 50, 50, 255));

	TransitionManager::getInstance().SetFadeOut(true);
}

void FirstScene::OnWindowEvent(const sf::Event& event)
{
}

void FirstScene::OnDraw(sf::RenderTarget& target)
{
	target.draw(m_map);

	LightManager::getInstance().clearUiLights();

	//UI Design

	sf::View savedView = target.getView();
	target.setView(target.getDefaultView());

	target.draw(m_healthBar);
	//target.draw(m_WeaponSlot);

	for (int i = 0; i < m_player->getCurrentHealth(); i++) {
		m_heart.setPosition(m_heartPos[i]);
		target.draw(m_heart);

		LightSource ls;
		ls.color = sf::Color(88, 199, 93, 255);
		ls.radius = 50.f;
		ls.isActive = true;
		sf::Vector2f midSize = sf::Vector2f((float)100 * m_heart.getScale().x / 2, 100 * m_heart.getScale().y / 2);
		LightManager::getInstance().addUiLight(m_heartPos[i] + midSize, ls);
	}

	for (int i = 0; i < m_player->getManaCount(); i++) {
		m_mana.setTextureRect(sf::IntRect(m_manaRect[i], { 200, 200 }));
		m_mana.setPosition(m_manaPos[i]);
		target.draw(m_mana);
	}

	for (int i = 0; i < m_weapons.size(); i++) {
		int plus = m_player->getCurentWeapon();
		if (i == plus)
			m_weapons[i].setScale(0.1, 0.1);
		else
			m_weapons[i].setScale(0.08, 0.08);
		m_weapons[i].setPosition(m_weaponPos[(i + plus) % 3]);
		target.draw(m_weapons[i]);
	}


	target.setView(savedView);
}

void FirstScene::zoom(float _factor, float _speed)
{
	m_onZoom = true;
	m_zoomSpeed = _speed;
	float dt = GameManager::Get()->GetDeltaTime();

	m_zoomFactor = pow(_factor, dt / _speed);

}

void FirstScene::OnUpdate()
{

	if (m_player->GetStateMachine().GetCurrentState() == 10)
		m_cam->setPosition(m_player->GetViseurPos());
	else
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
}

void FirstScene::FadeTransitionIn(float speed)
{
	m_alphaIn = 0;
	m_FadeSpeedIn = speed;
	m_radiusIn = 0;
}

void FirstScene::FadeTransitionOut(float speed)
{
	m_alphaOut = 255;
	m_FadeSpeedOut = speed;
	m_radiusOut = GetWindowHeight() * 1.25;
}
