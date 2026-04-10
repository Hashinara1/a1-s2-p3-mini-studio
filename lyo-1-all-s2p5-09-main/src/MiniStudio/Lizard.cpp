#include "AssetsManager.h"
#include "FirstScene.h"
#include "Lizard.h"
#include "LizardActions.h"
#include "LizardConditions.h"
#include "Utils.h"


void Lizard::OnInitialize()
{


	SetTexture(AssetsManager::getInstance().getTexture("Kobolt", "res/Monsters/Lizard2.png"));

	m_animator.loadFromJson("res/JsonAnimation/Kobolt.json");

	m_animator.applyScale(GetSprite(), { 20, 34 });

	IdleLizardAction* idle = m_stateMachine.CreateAction<IdleLizardAction>(0);
	PatrolLizardAction* patrol = m_stateMachine.CreateAction<PatrolLizardAction>(1);
	AttackLizardAction* attack = m_stateMachine.CreateAction<AttackLizardAction>(2);
	FleeLizardAction* flee = m_stateMachine.CreateAction<FleeLizardAction>(3);
	m_stateMachine.SetState(0);


	//condition Idle
	{
		//to patrol
		idle->CreateTransition(1)->AddCondition<PatrolCondition>(true);

		//to attack
		idle->CreateTransition(2)->AddCondition<AttackCondition>(true);

	}

	//condition patrol
	{
		//to attack
		patrol->CreateTransition(2)->AddCondition<AttackCondition>(true);
	}

	//condition attack
	{
		//to patrol
		attack->CreateTransition(1)->AddCondition<PatrolCondition>(true);
	}

}

void Lizard::OnFixedUpdate()
{
	m_stateMachine.Update();
	if (isDead())
	{
		m_stateMachine.SetState(3);
	}

}

bool Lizard::IsAttackFinished()
{
	return m_loopCount >= 4;
}

bool Lizard::IsPlayerInFront()
{
	sf::Vector2f playerPos  =  GetScene<FirstScene>()->m_player->GetPosition();
	sf::Vector2f lizardPos = GetPosition();
	float lizardDir = GetVelocity().x;

	float dist = Utils::GetDistance(playerPos.x, playerPos.y, lizardPos.x, lizardPos.y);

	float distX = abs(lizardPos.x - playerPos.x);
	float distY = abs(lizardPos.y - playerPos.y);


	if (lizardDir > 0)
	{
		if (playerPos.x > lizardPos.x)
		{
			if (distX <= 60)
			{
				std::cout << "attack1" << std::endl;
				return true;
			}
		}
		return false;
	}

	if (lizardDir < 0)
	{
		if (playerPos.x < lizardPos.x)
		{
			if (distX <= 60)
			{
				std::cout << "attack2" << std::endl;
				return true;
			}


		}
		return false;
	}


}

bool Lizard::IsPatrolFinished()
{
	return m_PatrolTimer >= 4;
}


void Lizard::Patrol(float dt)
{
	m_PatrolTimer += dt;

	if (m_PatrolTimer >= m_PatrolDuration)
	{
		m_PatrolTimer = 0;

		// new duration between 1 and 3 seconds
		m_PatrolDuration = 1.f + static_cast<float>(rand()) / RAND_MAX * 2.f;

		// change direction randomly
		m_PatrolDirection = (rand() % 2 == 0) ? 1 : -1;
	}

	SetVelocityX(m_PatrolDirection * GetSpeed());
}

void Lizard::Attack()
{

	if (IsColliding(GetScene<FirstScene>()->m_player))
	{
		GetScene<FirstScene>()->m_player->Damage(m_damage);
	}
}