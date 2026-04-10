#include "Enemies.h"
#include "EnemiesActions.h"
#include "AtkEntity.h"
#include "AssetsManager.h"


void Enemie1ActionAttack::OnStart(Enemie1* pOwner)
{
	AtkEntity* atk = pOwner->CreateEntity<AtkEntity>(sf::Vector2f(40, 40), sf::Color::Transparent);
	atk->SetRigidBody(false);
	atk->SetStatic(false);
	atk->SetAffectedByGravity(false);
	atk->SetAffectedByFriction(false);
	float dir = pOwner->GetSprite().getScale().x > 0 ? -1 : 1;
	atk->SetVelocityX(dir * 300);
	atk->SetMaxRunSpeed(999);
	sf::Vector2f playerpos = pOwner->GetPosition();
	atk->SetPosition(playerpos.x + 16 * dir, playerpos.y);
	atk->init(1, 1.8);
	atk->SetTexture(AssetsManager::getInstance().getTexture("AtkDistanceMob", "res/Monsters/AttackDistanceMob.png"));


	atk->GetAnimator().loadFromJson("res/JsonAnimation/AttackDistanceMob.json");
	atk->GetAnimator().applyScale(atk->GetSprite(), sf::Vector2f(120, 120));
	if (dir > 0)
	{
		atk->GetSprite().setScale(-abs(atk->GetSprite().getScale().x), atk->GetSprite().getScale().y);
		atk->SetSpriteOffsetFlipped(sf::Vector2f(-70, -110));
	}
	else {
		atk->GetSprite().setScale(abs(atk->GetSprite().getScale().x), atk->GetSprite().getScale().y);
		atk->SetSpriteOffsetFlipped(sf::Vector2f(-30, 0));
	}
		
		
	atk->SetSpriteOffset(sf::Vector2f(50, 90));
	atk->addEntity(pOwner);
	pOwner->resetTimer();
}


void Enemie2ActionAttack::OnStart(Enemie2* pOwner)
{
	std::cout << "ennemy2 attack" << std::endl;
	AtkEntity* atk = pOwner->CreateEntity<AtkEntity>(sf::Vector2f(150, 300), sf::Color::Transparent);
	atk->SetRigidBody(false);
	atk->SetStatic(false);
	atk->SetAffectedByGravity(false);
	atk->SetAffectedByFriction(false);

	int dir = pOwner->GetSprite().getScale().x < 0 ? 1 : -1;

	sf::Vector2f playerpos = pOwner->GetPosition();
	atk->SetPosition(playerpos.x + 60 * dir, playerpos.y);
	atk->init(1, 0.62);
	atk->SetTexture(AssetsManager::getInstance().getTexture("AttackFrontalMob", "res/Monsters/AttackFrontalMob.png"));

	atk->GetAnimator().loadFromJson("res/JsonAnimation/Monster2_AttackAnimationSprite.json");
	atk->GetAnimator().applyScale(atk->GetSprite(), sf::Vector2f(500, 500));
	atk->SetSpriteOffset(sf::Vector2f(-100, -100));
	atk->SetSpriteOffsetFlipped(sf::Vector2f(-100, -100));
	
	if (dir > 0)
	{
		atk->GetSprite().setScale(-abs(atk->GetSprite().getScale().x), atk->GetSprite().getScale().y);
	}
	atk->addEntity(pOwner);


	pOwner->ResetTimer();
}


void Enemie3ActionAttack::OnStart(Enemie3* pOwner)
{
	AtkEntity* atk = pOwner->CreateEntity<AtkEntity>(sf::Vector2f(300, 200), sf::Color::Transparent);
	atk->SetRigidBody(false);
	atk->SetStatic(false);
	atk->SetAffectedByGravity(false);
	atk->SetAffectedByFriction(false);

	int dir = pOwner->GetSprite().getScale().x < 0 ? -1 : 1;

	sf::Vector2f playerpos = pOwner->GetPosition();
	atk->SetPosition(playerpos.x + 16 * dir, playerpos.y);
	atk->init(1, 0.48);
	atk->SetTexture(AssetsManager::getInstance().getTexture("Monster3_AttackSprite", "res/Monsters/Monster3_AttackSprite.png"));

	atk->GetAnimator().loadFromJson("res/JsonAnimation/Monstre3.json");
	atk->GetAnimator().applyScale(atk->GetSprite(), sf::Vector2f(500, 500));
	atk->SetSpriteOffset(sf::Vector2f(-100, -120));
	atk->addEntity(pOwner);


	pOwner->ResetTimer();
}

