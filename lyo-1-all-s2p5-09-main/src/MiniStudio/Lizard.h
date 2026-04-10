#pragma once

#include "AliveEntity.h"
#include "StateMachine.h"

class Lizard : public AliveEntity
{
private:
	StateMachine<Lizard> m_stateMachine = StateMachine<Lizard>(this, 4);
	int m_waitTimer;
	int m_timer;
	int m_damage = 3;


public:
	
	int m_loopCount;
	int m_PatrolDirection = 1;

	float m_PatrolDuration = 0.f;
	float m_PatrolTimer = 0.f;

	int height = 96;
	int width = 148;
	
	int frame = 0;

	float inter = 0.1f;
	float timer = 0;

	void OnFixedUpdate() override;
	void OnInitialize() override;

	bool IsAttackFinished();
	bool IsPatrolFinished();
	bool IsPlayerInFront();

	void Attack();
	void Patrol(float dt);
};

