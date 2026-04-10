#include "AssetsManager.h"
#include "ShadowOfStorm.h"
#include "ShadowOfStormAction.h"
#include "ShadowOfStormCondition.h"

void ShadowOfStorm::SetUpStateMachine()
{

	IdleSOSAction* idle = m_stateMachine.CreateAction<IdleSOSAction>(0);
	RunSOSAction* run = m_stateMachine.CreateAction<RunSOSAction>(1);
	RushSOSAction* rush = m_stateMachine.CreateAction<RushSOSAction>(2);
	CACAttackSOSAction* atk = m_stateMachine.CreateAction<CACAttackSOSAction>(3);
	DistAttackSOSAction* thunderAtk = m_stateMachine.CreateAction<DistAttackSOSAction>(4);
	HealSOSAction* heal = m_stateMachine.CreateAction<HealSOSAction>(5);
	HitSOSAction* hit = m_stateMachine.CreateAction<HitSOSAction>(6);
	DeadSOSAction* dead = m_stateMachine.CreateAction<DeadSOSAction>(7);
	RunOppositeSOSAction* runO = m_stateMachine.CreateAction<RunOppositeSOSAction>(8);
	m_stateMachine.SetState(0);

	//Idle
	{

		//to Rush
		auto transition = idle->CreateTransition(2);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessRushRange>(true);
		transition->AddCondition<IsPlayerLessAttackRange>(false);
		transition->AddCondition<CanSOSRush>(true);

		//to atk
		transition = idle->CreateTransition(3);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessAttackRange>(true);
		transition->AddCondition<IsPlayerLessThunderRange>(false);
		transition->AddCondition<CanSOSAttack>(true);

		//to ThunderAtk
		transition = idle->CreateTransition(4);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessThunderRange>(true);
		transition->AddCondition<CanSOSThunder>(true);

		//to Heal
		transition = idle->CreateTransition(5);
		transition->AddCondition<IsPlayerCloseSOS>(false);
		transition->AddCondition<IsSOSFullHealth>(false);
		transition->AddCondition<CanSOSHeal>(true);

		//to HealBis
		transition = idle->CreateTransition(5);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessRushRange>(false);
		transition->AddCondition<IsSOSFullHealth>(false);
		transition->AddCondition<CanSOSHeal>(true);

		//to Run
		transition = idle->CreateTransition(1);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsSOSLesshalfHealth>(false);


		//to RunOpposite
		transition = idle->CreateTransition(8);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsSOSLesshalfHealth>(true);

	}

	//Run
	{
		//to Idle
		auto transition = run->CreateTransition(0);
		transition->AddCondition<IsPlayerCloseSOS>(false);

		//to Rush
		transition = run->CreateTransition(2);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessRushRange>(true);
		transition->AddCondition<IsPlayerLessAttackRange>(false);
		transition->AddCondition<CanSOSRush>(true);

		//to atk
		transition = run->CreateTransition(3);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessAttackRange>(true);
		transition->AddCondition<IsPlayerLessThunderRange>(false);
		transition->AddCondition<CanSOSAttack>(true);


		//to ThunderAtk
		transition = run->CreateTransition(4);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessThunderRange>(true);
		transition->AddCondition<CanSOSThunder>(true);

		//to RunOpposite //idem
		transition = run->CreateTransition(8);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsSOSFullHealth>(false);
		transition->AddCondition<CanSOSThunder>(false);
		transition->AddCondition<CanSOSAttack>(false);
		transition->AddCondition<CanSOSRush>(false);

	}

	//Rush
	{
		//to Run
		auto transition = rush->CreateTransition(1);
		transition->AddCondition<IsSOSAnimationFinish>(true);
	}

	//Attack
	{
		//to Idle
		auto transition = atk->CreateTransition(0);
		transition->AddCondition<IsPlayerCloseSOS>(false);
		transition->AddCondition<IsSOSAnimationFinish>(true);

		//to Run
		transition = atk->CreateTransition(1);
		transition->AddCondition<IsSOSAnimationFinish>(true);
		transition->AddCondition<IsPlayerCloseSOS>(true);

		//to Rush
		transition = atk->CreateTransition(2);
		transition->AddCondition<IsPlayerCloseSOS>(true);
		transition->AddCondition<IsPlayerLessRushRange>(false);
		transition->AddCondition<IsPlayerLessAttackRange>(false);
		transition->AddCondition<IsSOSAnimationFinish>(true);
		transition->AddCondition<CanSOSRush>(true);
	}

	//Thunder
	{
		//to Idle
		auto transition = thunderAtk->CreateTransition(0);
		transition->AddCondition<IsSOSAnimationFinish>(true);
	}

	//Heal
	{
		//to Idle
		auto transition = heal->CreateTransition(0);
		transition->AddCondition<IsSOSAnimationFinish>(true);
	}

	//Hit
	{
		//to Idle
		auto transition = hit->CreateTransition(0);
		transition->AddCondition<IsSOSAnimationFinish>(true);
		transition->AddCondition<IsSOSDead>(false);

		//To Dead
		transition = hit->CreateTransition(7);
		transition->AddCondition<IsSOSAnimationFinish>(true);
		transition->AddCondition<IsSOSDead>(true);

	}

	//RunOpposite
	{
		//to Idle
		auto transition = runO->CreateTransition(0);
		transition->AddCondition<IsPlayerCloseSOS>(false);

		transition = runO->CreateTransition(1);
		transition->AddCondition<CanSOSAttack>(true);

		transition = runO->CreateTransition(1);
		transition->AddCondition<CanSOSRush>(true);

		transition = runO->CreateTransition(1);
		transition->AddCondition<CanSOSHeal>(true);

		transition = runO->CreateTransition(1);
		transition->AddCondition<CanSOSThunder>(true);
	}
}

void ShadowOfStorm::AddTime(float _dt)
{
	m_rushTimer += _dt;
	m_healTimer += _dt;
	m_thunderTimer += _dt;
	m_attackTimer += _dt;
}

int ShadowOfStorm::GetDir()
{
	if (mSprite.getScale().x < 0)
		return -1;
	return 1;
}

void ShadowOfStorm::OnDamage()
{
	if (m_stateMachine.GetCurrentState() != 6 && m_stateMachine.GetCurrentState() != 7)
		m_stateMachine.SetState(6);
}

void ShadowOfStorm::OnFixedUpdate()
{
	m_stateMachine.Update();
	AddTime(GetDeltaTime());


	float VelX = GetVelocity().x;
	if (VelX < -1)
	{
		mSprite.setScale(-abs(mSprite.getScale().x), mSprite.getScale().y);
	}
	else  if (VelX > 1) {
		mSprite.setScale(abs(mSprite.getScale().x), mSprite.getScale().y);
	}

	m_animator.update(GetDeltaTime(), mSprite);
}

void ShadowOfStorm::OnCollision(Entity* collidedWith)
{}

void ShadowOfStorm::OnInitialize()
{
	SetRigidBody(true);
	SetAffectedByGravity(true);
	SetAffectedByFriction(true);

	setSpeed(150);

	SetTexture(AssetsManager::getInstance().getTexture("SOS", "res/SOS.png"));
	m_animator.loadFromJson("res/JsonAnimation/SOS.json");
	m_animator.applyScale(mSprite, { 500, 360 });
	SetSpriteOffset({ -90.f, -110.f });
	SetSpriteOffsetFlipped({ -140, -110.f });

	SetUpStateMachine();
}

void ShadowOfStorm::OnDestroy()
{}

bool ShadowOfStorm::IsAnimationFinish()
{
	return m_animator.isFinished();
}
