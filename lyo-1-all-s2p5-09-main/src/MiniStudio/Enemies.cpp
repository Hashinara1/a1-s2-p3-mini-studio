#include "Enemies.h"
#include "EnemiesActions.h"
#include "EnemiesConditions.h"
#include <iostream>
#include "AssetsManager.h"


void Enemie1::setUpStateMachine()
{
	Enemie1ActionIdle* idle = m_stateMachine.CreateAction<Enemie1ActionIdle>(0);
	Enemie1ActionAttack* atk = m_stateMachine.CreateAction<Enemie1ActionAttack>(1);
	m_stateMachine.SetState(0);
	//Ilde
	{
		//to atk
		idle->CreateTransition(1)->AddCondition<Enemie1IsPlayerFront>(true);
	}

	//attack
	{
		atk->CreateTransition(0)->AddCondition<Enemie1CanAttack>(true);
	}
}

void Enemie2::setUpStateMachine()
{
	Enemie2ActionIdle* idle = m_stateMachine.CreateAction<Enemie2ActionIdle>(0);
	Enemie2ActionAttack* atk = m_stateMachine.CreateAction<Enemie2ActionAttack>(1);
	m_stateMachine.SetState(0);
	//Ilde
	{
		//to atk
		idle->CreateTransition(1)->AddCondition<Enemie2IsPlayerFront>(true);

	}

	//attack
	{
		atk->CreateTransition(0)->AddCondition<Enemie2CanAttack>(true);
	}
}

void Enemie3::setUpStateMachine()
{
	Enemie3ActionIdle* idle = m_stateMachine.CreateAction<Enemie3ActionIdle>(0);
	Enemie3ActionAttack* atk = m_stateMachine.CreateAction<Enemie3ActionAttack>(1);
	m_stateMachine.SetState(0);
	//Ilde
	{
		//to atk
		idle->CreateTransition(1)->AddCondition<Enemie3IsPlayerFront>(true);

	}

	//attack
	{
		// to idle
		atk->CreateTransition(0)->AddCondition<Enemie3CanAttack>(true);
	}
}


void Enemie1::OnDestroy()
{
	//spawn de l'entité petit boom ou gros boom
}

void Enemie1::OnFixedUpdate()
{
	if (!m_player) {
		for (Entity* e : GameManager::Get()->getEntities()) {
			if (e->IsTag(9))
			{
				m_player = e;
				break;
			}
		}
	}

	if (m_player->GetPosition().x > GetPosition().x) {
		mSprite.setScale(-abs(mSprite.getScale().x), mSprite.getScale().y);
	}
	else {
		mSprite.setScale(abs(mSprite.getScale().x), mSprite.getScale().y);

	}

	m_timer += GetDeltaTime();

	m_stateMachine.Update();

}

void Enemie1::OnInitialize()
{

	SetTexture(AssetsManager::getInstance().getTexture("Mob1Mechant", "res/Monsters/Mob1_mechant.png"));
	mSprite.setScale(0.07, 0.07);
	SetSpriteOffset(sf::Vector2f(-20, -80));
	SetSpriteOffsetFlipped(sf::Vector2f(-20, -80));
	SetRigidBody(true);
	init(2);

	setUpStateMachine();


}

void Enemie1::OnDead()
{
	Destroy();
}

void Enemie2::OnFixedUpdate()
{
	if (!m_player)
	{
		for (Entity* e : GameManager::Get()->getEntities()) {
			if (e->IsTag(9))
			{
				m_player = e;
				break;
			}
		}
	}
	m_timer += GetDeltaTime();


	if (m_player->GetPosition().x < GetPosition().x) {
		//se retourner 
	}
	m_stateMachine.Update();
}
void Enemie2::OnInitialize()
{
	SetTexture(AssetsManager::getInstance().getTexture("Mob2Mechant", "res/Monsters/Mob2_mechant.png"));
	mSprite.setScale(0.07, 0.07);
	SetSpriteOffset(sf::Vector2f(-20, -40));
	SetRigidBody(true);
	init(2);
	setUpStateMachine();


}

void Enemie2::OnDead()
{
	Destroy();
}

void Enemie3::OnFixedUpdate()
{
	if (!m_player) {
		for (Entity* e : GameManager::Get()->getEntities()) {
			if (e->IsTag(9))
			{
				m_player = e;
				break;
			}
		}
	}

	m_timer += GetDeltaTime();

	m_stateMachine.Update();
}

void Enemie3::OnInitialize()
{

	SetTexture(AssetsManager::getInstance().getTexture("Mob3Mechant", "res/Monsters/Mob3_mechant.png"));
	mSprite.setScale(0.07, 0.07);
	SetSpriteOffset(sf::Vector2f(-20, -170));
	SetRigidBody(true);
	init(2);
	setUpStateMachine();
}

void Enemie3::OnDead()
{
	Destroy();
}
