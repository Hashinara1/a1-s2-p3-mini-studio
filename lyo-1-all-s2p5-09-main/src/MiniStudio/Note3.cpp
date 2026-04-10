#include "Note3.h"
#include "AssetsScene.h"
#include "Player.h"
#include "AssetsManager.h"

void Note3::OnInitialize()
{
	Notes::OnInitialize();
	lane = 3;

	SetTexture(AssetsManager::getInstance().getTexture("XButton", "res/Ui/XButton.png"));
	GetSprite().setScale(0.91f, 0.90f);


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

	float posX = player->GetPosition().x + 105;
	float posY = player->GetPosition().y - 200;

	SetPosition(posX, posY);
}

void Note3::OnFixedUpdate()
{
	Notes::OnFixedUpdate();
	
}

void Note3::OnCollision(Entity* collidedWith)
{
	
}

bool Note3::IsCollidingWithHitbar(Entity* collidedWith)
{
	if (IsColliding(collidedWith))
	{
		std::cout << "note 3 collided" << std::endl;
		if (ToDestroy())
			return false;

		Destroy();
		notesCount++;
		return true;
	}
	else
	{
		std::cout << "note 3 did not collided" << std::endl;
		return false;
	}
}
