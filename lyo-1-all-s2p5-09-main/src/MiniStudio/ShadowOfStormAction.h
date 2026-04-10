#pragma once
#include "Action.h"
#include "ShadowOfStorm.h"


class IdleSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class RunOppositeSOSAction : public Action<ShadowOfStorm> {

	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class RunSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class RushSOSAction : public Action<ShadowOfStorm>
{
	bool m_hitBoxSpawn;
	float m_lastMaxSpeed;
	float m_timer;

	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;

	void SpawnHitBox(ShadowOfStorm* s);
};

class CACAttackSOSAction : public Action<ShadowOfStorm>
{
	bool m_first;
	bool m_second;
	bool m_third;
	float m_timer;

	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;

	void spawnFirst(ShadowOfStorm* s);
	void spawnSecond(ShadowOfStorm* s);
	void spawnThird(ShadowOfStorm* s);
};

class DistAttackSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override {};
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class HealSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class HitSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

class DeadSOSAction : public Action<ShadowOfStorm>
{
	// Inherited via Action
	void OnStart(ShadowOfStorm* pOwner) override;
	void OnUpdate(ShadowOfStorm* pOwner) override;
	void OnEnd(ShadowOfStorm* pOwner) override;
};

