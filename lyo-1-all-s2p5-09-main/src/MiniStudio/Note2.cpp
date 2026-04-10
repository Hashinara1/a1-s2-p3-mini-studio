#include "Note2.h"
#include "AssetsScene.h"
#include "Player.h"
#include "AssetsManager.h"

void Note2::OnInitialize()
{
	Notes::OnInitialize();
	SetTexture(AssetsManager::getInstance().getTexture("BButton", "res/Ui/BButton.png"));
	GetSprite().setScale(0.61f, 0.74f);

	lane = 2;

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

	float posX = player->GetPosition().x + 70;
	float posY = player->GetPosition().y - 200;

	SetPosition(posX, posY);

}

void Note2::OnFixedUpdate()
{
	Notes::OnFixedUpdate();
	
}

void Note2::OnCollision(Entity* collidedWith)
{
}

bool Note2::IsCollidingWithHitbar(Entity* collidedWith)
{
	if (IsColliding(collidedWith))
	{
		std::cout << "note 2 collided" << std::endl;
		if (ToDestroy())
			return false;

		Destroy();
		notesCount++;
		return true;
	}
	else
	{
		std::cout << "note 2 collided" << std::endl;
		return false;
	}
}
