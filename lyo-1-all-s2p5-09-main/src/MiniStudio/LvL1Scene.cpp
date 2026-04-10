#include "LvL1Scene.h"
#include "LvL2Scene.h"
#include "Wall.h"
#include "Player.h"

#include "AssetsManager.h"
#include "LightManager.h"
#include <random>
#include "POPEntity.h"
#include "TransitionManager.h"
#include "ParallaxManager.h"
#include "Debug.h"
#include "FirstScene.h"
#include "Stele.h"

#include <random>

#include "Enemies.h"
#include "PauseUI.h"


void LvL1Scene::setUpUI()
{
	pauseUI = &PauseUI::Get();
	GameManager::Get()->addUI(pauseUI);

	m_loadingScreenSprite.setTexture(*AssetsManager::getInstance().getTexture("LoadingScreen", "res/Ui/LoadingScreen.png")->getTexture());
	m_loadingScreenSprite.setScale(2.4, 2.4);


	m_healthBar.setPosition(sf::Vector2f(0, 0));
	m_healthBar.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_healthBar.setTextureRect(sf::IntRect({ 0, 0 }, { 1175, 600 }));
	m_healthBar.setScale(sf::Vector2f(0.5, 0.5));

	m_heart.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_heart.setTextureRect(sf::IntRect({ 1796, 196 }, { 100, 100 }));
	m_heart.setScale(sf::Vector2f(0.5, 0.5));

	m_mana.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_mana.setScale(0.5, 0.5);

	m_WeaponSlot.setPosition(sf::Vector2f(0, 350));
	m_WeaponSlot.setTexture(*AssetsManager::getInstance().getTexture("FullUi", "res/Ui/Ui.png")->getTexture());
	m_WeaponSlot.setTextureRect(sf::IntRect({ 0, 1600 }, { 700, 700 }));
	m_WeaponSlot.setScale(0.5, 0.5);

	//permet d'inverser les couleur d'une texture
	m_invertShader.loadFromMemory(
		"void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; gl_FrontColor = gl_Color; }",
		"uniform sampler2D texture;"
		"void main() {"
		"    vec4 c = texture2D(texture, gl_TexCoord[0].xy);"
		"    gl_FragColor = vec4(1.0 - c.r, 1.0 - c.g, 1.0 - c.b, c.a);"
		"}"
	);
	m_invertShader.setUniform("texture", sf::Shader::CurrentTexture);
	//aucune idee de comment ca marche


	sf::Sprite sprite;
	sprite.setTexture(*AssetsManager::getInstance().getTexture("Guitar", "res/Ui/Attack.png")->getTexture());
	m_weapons.push_back(sprite);


	sprite.setTexture(*AssetsManager::getInstance().getTexture("Lyre", "res/Ui/Move.png")->getTexture());
	m_weapons.push_back(sprite);

	sprite.setTexture(*AssetsManager::getInstance().getTexture("Tambourin", "res/Ui/Enigme.png")->getTexture());
	m_weapons.push_back(sprite);


	m_heartPos = { {51, 57}, {128, 55}, {206, 56}, {290, 55}, {370, 55}, {450, 57} };
	m_manaPos = { {80, 90}, {95, 96}, {98, 102}, {110, 110}, {131, 110} };
	m_manaRect = { {2515, 2000}, {2545, 1560} , {2545, 1135}, {2515, 675} , {2555, 175} };

	m_weaponPos = { {-430, 160}, {-410, 340}, {-255, 340} };


	m_deathScreenSprite.setTexture(*AssetsManager::getInstance().getTexture("DeathScreen", "res/DeathScreen/ecran de mort.png")->getTexture());
	m_deathScreenSprite.setPosition({ 0,0 });
}

void LvL1Scene::DrawUI(sf::RenderTarget& target)
{

	LightManager::getInstance().clearUiLights();


	

	sf::View savedView = target.getView();
	target.setView(target.getDefaultView());

	if (!m_rock)
	{
		target.draw(m_healthBar);
		sf::RenderStates invertStates;
		invertStates.shader = &m_invertShader;
		target.draw(m_WeaponSlot, invertStates);

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



		//arme actuel
		LightSource ls;
		ls.color = sf::Color(109, 135, 119, 255);
		ls.radius = 150.f;
		ls.isActive = true;
		LightManager::getInstance().addUiLight(sf::Vector2f(180, 500), ls);


		//mana
		ls;
		ls.color = sf::Color::Cyan;
		ls.radius = 105.f;
		ls.isActive = true;
		LightManager::getInstance().addUiLight(sf::Vector2f(150, 150), ls);


		for (int i = 0; i < m_weapons.size(); i++) {
			int plus = m_player->getCurentWeapon();
			int posIndex = (i + plus) % 3;

			if (posIndex == 0)
				m_weapons[i].setScale(1.2f, 1.2f);
			else
				m_weapons[i].setScale(1.f, 1.f);

			m_weapons[i].setPosition(m_weaponPos[posIndex]);
			target.draw(m_weapons[i]);
		}

	}
	if (m_player->getDeathScreenShow())
		target.draw(m_deathScreenSprite);


	target.setView(savedView);

}

void LvL1Scene::ParseMap()
{
	m_map.load("res/maps/LvL1.tmx");

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
					m_player->setSpawnPoint(sf::Vector2f(obj.x, obj.y));
					m_rock->SetPosition(obj.x-40, obj.y - 400);

				}

				if (obj.name == "Stele") {
					Stele* tmp = CreateEntity<Stele>({ 75, 260 }, sf::Color::Transparent);
					tmp->SetPosition(obj.x, obj.y);
					tmp->setText(std::get<std::string>(obj.proprieties.at("Story")));

				}

				if (obj.name == "SpawnEnemy1") {
					Enemie1* tmp = CreateEntity<Enemie1>(sf::Vector2f(80, 200), sf::Color::Red);
					tmp->SetPosition(obj.x, obj.y);
				}
				if (obj.name == "SpawnEnemy2") {
					Enemie2* tmp = CreateEntity<Enemie2>(sf::Vector2f(80, 240), sf::Color::Transparent);
					tmp->SetPosition(obj.x, obj.y);
				}
				if (obj.name == "SpawnEnemy3") {
					Enemie3* tmp = CreateEntity<Enemie3>(sf::Vector2f(80, 100), sf::Color::Transparent);
					tmp->SetPosition(obj.x, obj.y);
				}


			}
		}
	}
}

void LvL1Scene::InitPlayer()
{
	m_player = CreateEntity<Player>({ 60, 150 }, sf::Color::White);
	m_player->init(6);
	
	m_player->SetTexture(AssetsManager::getInstance().getTexture("AssetsPlayer", "res/Player/playerFinal.png"));
	m_player->SetRigidBody(true);
	m_player->setSpeed(2500);
	m_player->SetMaxRunSpeed(700);
	m_player->SetMaxFallSpeed(800);

	m_player->GetAnimator().loadFromJson("res/JsonAnimation/playerFinal.json");
	m_player->SetSpriteOffset({ -35.f, -100.f });
	m_player->SetSpriteOffsetFlipped({ -55, -100.f });

	m_player->GetAnimator().applyScale(m_player->GetSprite(), { 120, 340 });
	m_player->GetAnimator().play("Idle");


	m_rock = CreateEntity<Rock>(5, sf::Color::Transparent);
	m_rock->init(7);
	m_rock->setCallBack([this]() { m_rock = nullptr; m_qte->setActive(false); });
	m_player->setActive(false);

	m_qte = CreateEntity<QTEEntity>(1, sf::Color::Transparent);
}

void LvL1Scene::InitCam()
{
	m_cam->setPosition(m_player->GetPosition());
	GameManager::Get()->setCamera(*m_cam);

	ParallaxManager::getInstance().setCamera(m_cam);



	m_onZoom = false;
	m_zoomTimer = 0.f;

	m_cam->zoom(6.8);
	
}

void LvL1Scene::lunchWaitingScreen()
{
	TransitionManager::getInstance().SetCallBack(nullptr);

	m_loading = true;
	m_player->SetPosition(0, 0);
	m_timer = 0;
}

void LvL1Scene::OnInitialize()
{

	//init sounds
	m_audioMap->addSound("FootStep", { "res/Audio/Sounds/Step/foot.mp3" });
	m_audioMap->setVolume("FootStep", 0.3);
	m_audioMap->addSound("Dash", { "res/Audio/Sounds/Whoosh & Slash/Whoosh.wav" });
	m_audioMap->addSound("AddManaSound", { "res/Audio/Sounds/Magic & Skill/Fx.wav" });
	m_audioMap->setVolume("AddManaSound", 0.5);
	m_audioMap->addSound("Ambiance", { "res/Audio/Sounds/Ambient/AmbianceSound.wav" });
	m_audioMap->setVolume("Ambiance", 0.5);

	
	//init Paralax
	ParallaxManager::getInstance().init("res/Background/BackgroundCave");
	

	InitPlayer();
	ParseMap();
	InitCam();
	setUpUI();


	SetGravity({ 0, 12 });

	TransitionManager::getInstance().SetFadeOut(true);


}

void LvL1Scene::OnWindowEvent(const sf::Event& event)
{
}

void LvL1Scene::OnUpdate()
{
	m_audioMap->playSound("Ambiance");

	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Plus))
	{
		GameManager::Get()->SetPause(true);
		pauseUI->SetActif(true);
	}


	if (m_rock) {
		if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Sud)) {
			m_rock->Damage(1);
			m_cam->zoom(0.9);
		}
	}

	m_cam->setPosition(m_player->GetPosition());
	m_cam->CorrectPosition(sf::Vector2f(m_map.width * m_map.tileWidth, m_map.height * m_map.tileHeight));

	if (m_qte && m_qte->isActive())
	{
		sf::View v = m_cam->getView();
		m_qte->SetPosition(v.getCenter().x, v.getCenter().y - 75);
	}
	GameManager::Get()->setCamera(*m_cam);

	float pos = m_player->GetPosition(0, 0).x;
	if (pos > m_map.width * m_map.tileWidth && TransitionManager::getInstance().IsFadingIn() == false)
	{
		TransitionManager::getInstance().SetFadeIn(true);
		TransitionManager::getInstance().SetCallBack([this]() { lunchWaitingScreen(); });
		m_player->SetRight(Player::Action::Move, false);
		m_player->SetVelocityY(0);
		m_player->SetAffectedByGravity(false);
		m_player->SetRigidBody(false);
		
	}

	if (m_loading) {
		m_timer += GetDeltaTime();
		if (m_timer > 3)
			GameManager::Get()->LaunchScene<LvL2Scene>();
	}
}

void LvL1Scene::OnDraw(sf::RenderTarget& target)
{
	target.draw(m_map);


	if (m_loading)
		target.draw(m_loadingScreenSprite);
}
