#include "PauseUI.h"
#include "InputManager.h"
#include "BoutonAction.h"
#include "Utils.h"
#include "Entity.h"
#include "Debug.h"
#include "AssetsManager.h"
#include "LightManager.h"

PauseUI& PauseUI::Get() {
	static PauseUI instance;
	return instance;
}

PauseUI::PauseUI() :
	UI(),
	target(nullptr),
	mAccumulatedDeltaTime(0.f),
	m_inter(0.5f),
	inputManager(&InputManager::getInstance()),
	nearest(nullptr),
	dist(0),
	start(false),
	end(false),
	decPosOut(GetWindowWidth() * 2),
	decPosCurrent(GetWindowWidth() * 2)
{
}
PauseUI::~PauseUI()
{

}
void PauseUI::OnInit()
{
	float mult = 2.5;
	float width = GetWindowWidth() * mult;
	float height = GetWindowHeight() * mult;
	//	Create the entity from back to front
	std::shared_ptr<Entity> backBoard = CreateSharedEntity<Entity>(
		sf::Vector2f(width, height), sf::Color::White);
	backBoard.get()->SetPosition(sf::Vector2f(GetWindowWidth() / 2, GetWindowHeight() / 2));
	backBoard.get()->SetTexture(AssetsManager::getInstance().getTexture("pause pierre 3", "res/Ui/pause pierre 3.png"));
	backBoard.get()->GetSprite().setScale(sf::Vector2f(mult, mult));

	std::shared_ptr<Entity> backBoard1 = CreateSharedEntity<Entity>(
		sf::Vector2f(width, height), sf::Color::White);
	backBoard1.get()->SetPosition(sf::Vector2f(GetWindowWidth() / 2, GetWindowHeight() / 2));
	backBoard1.get()->SetTexture(AssetsManager::getInstance().getTexture("pause ornement 2", "res/Ui/pause ornement 2.png"));
	backBoard1.get()->GetSprite().setScale(sf::Vector2f(mult, mult));

	//	Cursor	
	Cursor = CreateSharedEntity<Entity>(
		sf::Vector2f(GetWindowWidth() * 2, GetWindowHeight() * 2), sf::Color::White);
	Cursor.get()->SetPosition(sf::Vector2f(GetWindowWidth() / 2, GetWindowHeight() / 2));
	Cursor.get()->SetTexture(AssetsManager::getInstance().getTexture("pause curseur 1", "res/Ui/pause curseur 1.png"));
	Cursor.get()->GetSprite().setScale(sf::Vector2f(2, 2));


	//	create boutons && their action && position
	// need change of the BoutonAction
	//	besoin d'utilisation du tile
	Bouton* boutonExit = new Bouton(CreateSharedEntity<Entity>(
		sf::Vector2f(GetWindowWidth() / 4, GetWindowHeight() / 4), sf::Color::White),
		new BoutonActionExitPauseMenu());
	boutonExit->pos = sf::Vector2f(GetWindowWidth() / 4, GetWindowHeight() / 2);
	boutonExit->mEntity.get()->SetPosition(boutonExit->pos);
	boutonExit->mEntity.get()->SetTexture(AssetsManager::getInstance().getTexture("quitter", "res/Ui/quitter.png"));
	boutonExit->mEntity.get()->GetSprite().setScale(sf::Vector2f(mult, mult));
	boutonExit->mEntity.get()->SetSpriteOffset(sf::Vector2f{ -1500,-1100 });
	boutonExit->mAction->SetMyParentUI(this);
	m_boutons.push_back(boutonExit);
	//	besoin d'utilisation du tile
	Bouton* boutonContinue = new Bouton(CreateSharedEntity<Entity>(
		sf::Vector2f(GetWindowWidth() / 4, GetWindowHeight() / 4), sf::Color::White),
		new BoutonActionContinuePauseMenu());
	boutonContinue->pos = sf::Vector2f(GetWindowWidth() / 4, -GetWindowHeight() / 2);
	boutonContinue->mEntity.get()->SetPosition(boutonExit->pos);
	boutonContinue->mEntity.get()->SetTexture(AssetsManager::getInstance().getTexture("continuer", "res/Ui/continuer.png"));
	boutonContinue->mEntity.get()->GetSprite().setScale(sf::Vector2f(mult, mult));
	boutonContinue->mEntity.get()->SetSpriteOffset(sf::Vector2f{ -1500,-550 });
	boutonContinue->mAction->SetMyParentUI(this);
	m_boutons.push_back(boutonContinue);


	LightSource ls;
	ls.color = { 255,255,255,150 };
	ls.radius = 10000;
	LightManager::getInstance().addLight(Cursor.get());
	Cursor.get()->setLightSource(ls);

	//	set rotation pour le cursor vers le bouton de base

	target = boutonContinue;


}
void PauseUI::FixedUpdate()
{
	if (start || end)
	{
		if (end)
			start = false;
		if (start)
		{
			decPosCurrent -= 100;
			if (decPosCurrent <= 0)
			{
				decPosCurrent = 0;
				start = false;
			}
		}
		else
		{
			decPosCurrent += 100;
			if (decPosCurrent >= decPosOut)
			{
				decPosCurrent = decPosOut;
				end = false;
				start = true;
				UI::SetActif(false);
				return;
			}
		}

		//for (std::shared_ptr<Entity> it : m_entities)
			//it.get()->SetSpriteOffset(sf::Vector2f(decPosCurrent, 0));
	}
	sf::Vector2f UiPos = (sf::Vector2f)mpGameManager->GetScene()->getCam().get()->getView().getCenter();
	sf::Vector2f move = UiPos + sf::Vector2f(decPosCurrent, 0);

	for (std::shared_ptr<Entity> it : m_entities)
	{
		it.get()->SetPosition(move);
	}
	for (Bouton* it : m_boutons)
	{
		it->mEntity.get()->SetPosition(move + it->pos);
	}

	sf::Vector2f targetPos = target->mEntity.get()->GetPosition();

	if (Cursor.get()->GetPosition().y != targetPos.y)
	{
		float dist = targetPos.y - Cursor.get()->GetPosition().y;
		if (std::abs(dist) < 5)
			Cursor.get()->SetPosition(sf::Vector2f(targetPos.x - 100, targetPos.y));
		else
		{
			dist /= 4;
			Cursor.get()->SetPosition(sf::Vector2f(targetPos.x - 100, Cursor.get()->GetPosition().y + dist));
		}
	}
	else
	{
		Cursor.get()->SetPosition(sf::Vector2f(targetPos.x - 100, targetPos.y));
	}


	mAccumulatedDeltaTime += mpGameManager->GetDeltaTime();
	OnWindowEvent();
}
void PauseUI::OnWindowEvent()
{
	//	Debug Clavier
	if (inputManager->isKeyRelease(sf::Keyboard::Enter))
	{
		UpdateInput(InputAction::Comfirm);
		return;
	}

	//	Mannette
	if (inputManager->isJoystickButtonRelease(JoystickButton::Est))
	{
		UpdateInput(InputAction::Comfirm);
		return;
	}

	if (mAccumulatedDeltaTime < m_inter)
		return;

	nearest = nullptr;

	//	Debug Clavier
	if (inputManager->isKeyRelease(sf::Keyboard::Left))
		UpdateInput(InputAction::Left);
	else if (inputManager->isKeyRelease(sf::Keyboard::Right))
		UpdateInput(InputAction::Right);
	else if (inputManager->isKeyRelease(sf::Keyboard::Up))
		UpdateInput(InputAction::Up);
	else if (inputManager->isKeyRelease(sf::Keyboard::Down))
		UpdateInput(InputAction::Down);

	//	Mannette
	float joystick = inputManager->getAxisValue(JoystickAxis::LeftJoystickX);
	if (joystick < -75 || joystick > 75)
		if (joystick < -75)	//	left
			UpdateInput(InputAction::Left);
		else				//	right
			UpdateInput(InputAction::Right);
	else
	{
		joystick = inputManager->getAxisValue(JoystickAxis::LeftJoystickY);
		if (joystick < -75 || joystick > 75)
			for (Bouton* b : m_boutons)
				if (joystick < -75)	//	up
					UpdateInput(InputAction::Up);
				else				//	down
					UpdateInput(InputAction::Down);
		else		//	no input valid
			return;
	}
	if (!nearest)
		return;
	target = nearest;

	//	reset timer
	mAccumulatedDeltaTime = 0;
}

void PauseUI::SetActif(bool it)
{
	if (it)
	{
		UI::SetActif(it);
		end = false;
		start = true;
	}
	else
	{
		end = true;
	}
}
void PauseUI::UpdateInput(InputAction input)
{
	switch (input)
	{
	case Comfirm:
		target->mAction->Action();
		mAccumulatedDeltaTime = 0;
		break;
	case Left:
		for (Bouton* b : m_boutons)
		{
			if (b == target)
				continue;

			if (b->mEntity->GetPosition().x < target->mEntity->GetPosition().x)
			{
				if (!nearest)
				{
					nearest = b;
					dist = Utils::GetDistance(target->mEntity->GetPosition(), nearest->mEntity->GetPosition());
					continue;
				}
				float tmp = Utils::GetDistance(target->mEntity->GetPosition(), b->mEntity->GetPosition());
				if (tmp < dist)
				{
					nearest = b;
				}
			}
			else
				return;
		}
		if (nearest)
			target = nearest;
		break;
	case Right:
		for (Bouton* b : m_boutons)
		{
			if (b == target)
				continue;

			if (b->mEntity->GetPosition().x > target->mEntity->GetPosition().x)
			{
				if (!nearest)
				{
					nearest = b;
					dist = Utils::GetDistance(target->mEntity->GetPosition(), nearest->mEntity->GetPosition());
					continue;
				}
				float tmp = Utils::GetDistance(target->mEntity->GetPosition(), b->mEntity->GetPosition());
				if (tmp < dist)
				{
					nearest = b;
				}
			}
			else
				return;
		}
		if (nearest)
			target = nearest;

		break;
	case Up:
		for (Bouton* b : m_boutons)
		{
			if (b == target)
				continue;

			if (b->mEntity->GetPosition().y < target->mEntity->GetPosition().y)
			{
				if (nearest == nullptr)
				{
					nearest = b;
					dist = Utils::GetDistance(target->mEntity->GetPosition(), nearest->mEntity->GetPosition());
					continue;
				}
				float tmp = Utils::GetDistance(target->mEntity->GetPosition(), b->mEntity->GetPosition());
				if (tmp < dist)
				{
					nearest = b;
				}
			}
			else
				return;
		}
		if (nearest)
			target = nearest;

		break;
	case Down:
		for (Bouton* b : m_boutons)
		{
			if (b == target)
				continue;

			if (b->mEntity->GetPosition().y > target->mEntity->GetPosition().y)
			{
				if (nearest == nullptr)
				{
					nearest = b;
					dist = Utils::GetDistance(target->mEntity->GetPosition(), nearest->mEntity->GetPosition());
					continue;
				}
				float tmp = Utils::GetDistance(target->mEntity->GetPosition(), b->mEntity->GetPosition());
				if (tmp < dist)
				{
					nearest = b;
				}
			}
			else
				return;
		}
		if (nearest)
			target = nearest;

		break;
	default:
		break;
	}
}