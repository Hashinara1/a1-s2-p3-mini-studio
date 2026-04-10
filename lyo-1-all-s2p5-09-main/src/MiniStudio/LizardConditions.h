#pragma once
#include "Condition.h"
#include "Lizard.h"


class IsLizardDead : public Condition<Lizard>
{
public:
	bool OnTest(Lizard* pOwner) override {
		return pOwner->isDead();
	}
};

class IsLizardFinishStanding : public Condition<Lizard> {

public:
	bool OnTest(Lizard* pOwner)override {
		return false;

	}
};

class PatrolCondition : public Condition<Lizard> {

public:
	bool OnTest(Lizard* pOwner) override {
		return pOwner->IsAttackFinished();
	}
};

class AttackCondition : public Condition<Lizard> {

public:
	bool OnTest(Lizard* pOwner) override {
		return pOwner->IsPlayerInFront();
	}
};

class IsPatrolFinishedCondition : public Condition<Lizard> {

public:
	bool OnTest(Lizard* pOwner) override {
		return pOwner->IsPatrolFinished();
	}
};

