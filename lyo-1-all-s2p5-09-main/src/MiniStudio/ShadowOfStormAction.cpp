#include "AtkEntity.h"
#include "AssetsManager.h"
#include "Player.h"
#include "ShadowOfStormAction.h"


void IdleSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Idle");

}

void IdleSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	sf::Vector2f playerPos = pOwner->getPlayer()->GetPosition();
	sf::Vector2f SOSPos = pOwner->GetPosition();

	if (playerPos.x < SOSPos.x)
		pOwner->GetSprite().setScale(-abs(pOwner->GetSprite().getScale().x), pOwner->GetSprite().getScale().y);
	else
		pOwner->GetSprite().setScale(abs(pOwner->GetSprite().getScale().x), pOwner->GetSprite().getScale().y);
}

void IdleSOSAction::OnEnd(ShadowOfStorm* pOwner)
{}

void RunSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Run");

}

void RunSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	sf::Vector2f playerPos = pOwner->getPlayer()->GetPosition();
	sf::Vector2f SOSPos = pOwner->GetPosition();

	if (playerPos.x < SOSPos.x)
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * -1, 0));
	else
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * 1, 0));
}

void RunSOSAction::OnEnd(ShadowOfStorm* pOwner)
{}

void RushSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	m_lastMaxSpeed = pOwner->GetMaxRunSpeed();
	pOwner->GetAnimator().play("RushAndHit");


	sf::Vector2f playerPos = pOwner->getPlayer()->GetPosition();
	sf::Vector2f SOSPos = pOwner->GetPosition();


	pOwner->SetMaxRunSpeed(m_lastMaxSpeed * 3);
	if (playerPos.x < SOSPos.x)
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * -3, 0));
	else
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * 3, 0));
	m_timer = 0;
	m_hitBoxSpawn = false;
}

void RushSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	m_timer += pOwner->GetDeltaTime();

	if (m_timer > 1.24) 
		SpawnHitBox(pOwner);

	if (m_timer > 1.44)
		pOwner->SetVelocityX(0);
}

void RushSOSAction::OnEnd(ShadowOfStorm* pOwner)
{
	pOwner->SetMaxRunSpeed(m_lastMaxSpeed);
	pOwner->resetRush();
}

void RushSOSAction::SpawnHitBox(ShadowOfStorm* s)
{
	if (m_hitBoxSpawn)
		return;
	m_hitBoxSpawn = true;
	AtkEntity* hb = s->CreateEntity<AtkEntity>(sf::Vector2f(230, 90), sf::Color::Transparent);
	hb->SetPosition(s->GetPosition().x - 20, s->GetPosition().y);
	hb->init(1, 0.15);
	hb->addEntity(s);
}

void CACAttackSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Attack");


	m_first = false;
	m_second = false;
	m_third = false;
	m_timer = 0;
}

void CACAttackSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	m_timer += pOwner->GetDeltaTime();

	if (m_timer >= 0.60)
		spawnFirst(pOwner);


	if (m_timer >= 1.2)
		spawnSecond(pOwner);


	if (m_timer >= 1.6)
		spawnThird(pOwner);
}

void CACAttackSOSAction::OnEnd(ShadowOfStorm* pOwner)
{
	pOwner->resetAttack();
}

void CACAttackSOSAction::spawnFirst(ShadowOfStorm* s)
{
	if (m_first)
		return;
	m_first = true;

	AtkEntity* hb = s->CreateEntity<AtkEntity>(sf::Vector2f(230, 90), sf::Color::Transparent);
	hb->SetPosition(s->GetPosition().x - 20, s->GetPosition().y);
	hb->init(1, 0.15);
	hb->addEntity(s);
}

void CACAttackSOSAction::spawnSecond(ShadowOfStorm* s)
{
	if (m_second)
		return;
	m_second = true;

	AtkEntity* hb = s->CreateEntity<AtkEntity>(sf::Vector2f(120, 160), sf::Color::Transparent);
	hb->SetPosition(s->GetPosition().x, s->GetPosition().y-40);
	hb->init(1, 0.15);
	hb->addEntity(s);
}

void CACAttackSOSAction::spawnThird(ShadowOfStorm* s)
{
	if (m_third)
		return;
	m_third = true;

	AtkEntity* hb = s->CreateEntity<AtkEntity>(sf::Vector2f(230, 90), sf::Color::Transparent);
	hb->SetPosition(s->GetPosition().x - 20, s->GetPosition().y);
	hb->init(1, 0.15);
	hb->addEntity(s);
}

void DistAttackSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Thunder");
}

void DistAttackSOSAction::OnEnd(ShadowOfStorm* pOwner)
{
	AtkEntity* hb = pOwner->CreateEntity<AtkEntity>(sf::Vector2f(30, 90), sf::Color::Transparent);
	hb->SetPosition(pOwner->GetPosition().x + pOwner->GetDir() * 75, pOwner->GetPosition().y);
	hb->init(2, 0.15);
	hb->addEntity(pOwner);
	hb->SetTexture(AssetsManager::getInstance().getTexture("Foudre", "res/Monsters/foudre-de-SOS.png"));
	hb->GetAnimator().loadFromJson("res/JsonAnimation/foudre.json");
	hb->GetAnimator().applyScale(hb->GetSprite(), sf::Vector2f(230, 90));

	pOwner->resetThunder();
}

void HealSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Heal");

}

void HealSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{}

void HealSOSAction::OnEnd(ShadowOfStorm* pOwner)
{
	if (pOwner->IsAnimationFinish())
		pOwner->Heal(10);
	pOwner->resetHeal();
}

void HitSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Damage");

}

void HitSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{}

void HitSOSAction::OnEnd(ShadowOfStorm* pOwner)
{}

void DeadSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Death");

}

void DeadSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	if (pOwner->IsAnimationFinish())
		pOwner->Destroy();
}

void DeadSOSAction::OnEnd(ShadowOfStorm* pOwner)
{}

void RunOppositeSOSAction::OnStart(ShadowOfStorm* pOwner)
{
	pOwner->GetAnimator().play("Run");

}

void RunOppositeSOSAction::OnUpdate(ShadowOfStorm* pOwner)
{
	sf::Vector2f playerPos = pOwner->getPlayer()->GetPosition();
	sf::Vector2f SOSPos = pOwner->GetPosition();

	if (playerPos.x > SOSPos.x)
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * -1, 0));
	else
		pOwner->AddVelocity(sf::Vector2f(pOwner->GetSpeed() * 1, 0));
}

void RunOppositeSOSAction::OnEnd(ShadowOfStorm* pOwner)
{}
