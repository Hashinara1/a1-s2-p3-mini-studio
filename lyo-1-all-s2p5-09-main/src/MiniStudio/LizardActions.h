#pragma once

#include "Action.h"
#include "Lizard.h"

class  IdleLizardAction : public Action<Lizard>
{
	int m_maxFrame = 4;

	// Herite via Action
	void OnStart(Lizard* pOwner) override;
	void OnUpdate(Lizard* pOwner) override;
	void OnEnd(Lizard* pOwner) override;
};

class PatrolLizardAction : public Action<Lizard> {

	int m_maxFrame = 6;
	// Herite via Action
	void OnStart(Lizard* pOwner) override;
	void OnUpdate(Lizard* pOwner) override;
	void OnEnd(Lizard* pOwner) override;
};

class AttackLizardAction : public Action<Lizard> {

	int m_maxFrame = 3;

	// Herite via Action
	void OnStart(Lizard* pOwner) override;
	void OnUpdate(Lizard* pOwner) override;
	void OnEnd(Lizard* pOwner) override;

	friend class Lizard;
};

class FleeLizardAction : public Action<Lizard> {

	int m_maxFrame = 6;

	// Herite via Action
	void OnStart(Lizard* pOwner) override;
	void OnUpdate(Lizard* pOwner) override;
	void OnEnd(Lizard* pOwner) override;
};
