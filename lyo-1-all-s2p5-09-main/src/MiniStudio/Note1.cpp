#include "Note1.h"
#include "AssetsScene.h"
#include "Player.h"
#include "AssetsManager.h"
#include "Debug.h"

void Note1::OnInitialize()
{
	SetTexture(AssetsManager::getInstance().getTexture("AButton", "res/Ui/AButton.png"));
	GetSprite().setScale(0.58f, 0.59f);

	Notes::OnInitialize();
	lane = 1;

	Player* player = nullptr;


	for (Entity* e : GameManager::Get()->getEntities()) {
		if (e->IsTag(9))
		{
			player = dynamic_cast<Player*>(e);
			break;
		}
	}

	if (player == nullptr)
		return;

	float posX = player->GetPosition().x + 50;
	float posY = player->GetPosition().y - 200;

	
	LightSource l1;
	l1.color = sf::Color::Green;
	l1.radius = 30.f;
	
	l1.isActive = true;
	
	setLightSource(l1);
	SetPosition(posX, posY);
}

void Note1::OnFixedUpdate()
{
	Notes::OnFixedUpdate();

}

void Note1::OnCollision(Entity* collidedWith)
{
	
}

bool Note1::IsCollidingWithHitbar(Entity* collidedWith)
{
	if (IsColliding(collidedWith) )
	{
		std::cout << "note 1 collided" << std::endl;
		if (ToDestroy())
			return false;

		Destroy();
		notesCount++;
		return true;
	}
	else
	{
		std::cout << "note 1 did not collided" << std::endl;
		return false;
	}
}

