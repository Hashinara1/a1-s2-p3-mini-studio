#include "Note4.h"
#include "AssetsScene.h"
#include "AssetsManager.h"
#include "Player.h"
void Note4::OnInitialize()
{
	Notes::OnInitialize();
	lane = 4;
	SetTexture(AssetsManager::getInstance().getTexture("YButton", "res/Ui/YButton.png"));
	GetSprite().setScale(1.63f, 1.45f);

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

	float posX = player->GetPosition().x + 200;
	float posY = player->GetPosition().y - 200;

	LightSource l1;
	l1.color = sf::Color::Yellow;
	l1.radius = 30.f;
	l1.isActive = true;
	setLightSource(l1);

	SetPosition(posX, posY);

}

void Note4::OnFixedUpdate()
{
	Notes::OnFixedUpdate();

}

void Note4::OnCollision(Entity* collidedWith)
{
	
}

bool Note4::IsCollidingWithHitbar(Entity* collidedWith)
{
	if (IsColliding(collidedWith))
	{
		std::cout << "note 4 collided" << std::endl;
		//m_audioMap->playSound("Note4");
		if (ToDestroy())
			return false;

		Destroy();
		notesCount++;
		return true;
	}
	else
	{
		std::cout << "note 4 did not collided" << std::endl;
		return false;
	}
}

