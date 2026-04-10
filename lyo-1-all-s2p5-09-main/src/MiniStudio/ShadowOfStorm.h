#pragma once

#include "AliveEntity.h"
#include "StateMachine.h"

class Player;

class ShadowOfStorm : public AliveEntity
{

private:
	
	float m_detectionRadius = 350;

	float m_range = 180;
	float m_rushRange = 300;
	float m_thunderRange = 50;


	float m_attackTimer = 0;
	float m_healTimer = 0;
	float m_rushTimer = 0;
	float m_thunderTimer = 0;



	float m_attackCD = 1;
	float m_healCD = 15;
	float m_rushCD = 8;
	float m_thunderCD = 5;

	Player* m_player;

	StateMachine<ShadowOfStorm> m_stateMachine = StateMachine<ShadowOfStorm>(this, 9);

	void SetUpStateMachine();

	void AddTime(float _dt);

public:

	void SetPlayer(Player* _player) { m_player = _player; }

	Player* getPlayer() { return m_player; }

	int GetDir();

	float getAttackRange() const { return m_range; }
	float getDetectionRadius() const { return m_detectionRadius; }
	float getRushRange() const { return m_rushRange; }
	float getThunderRange() const { return m_thunderRange; }
	
	void OnCollision(Entity* collidedWith) override;
	void OnDamage() override;
	void OnDestroy() override;
	void OnFixedUpdate() override;
	void OnInitialize() override;

	bool canAttack() const { return m_attackTimer >= m_attackCD; }
	bool canHeal() const { return m_healTimer >= m_healCD; }
	bool canRush() const { return m_rushTimer >= m_rushCD; }
	bool canThunder() const { return m_thunderTimer >= m_thunderCD; }

	void resetAttack() { m_attackTimer = 0; }
	void resetHeal() { m_healTimer = 0; }
	void resetRush() { m_rushTimer = 0; }
	void resetThunder() { m_thunderTimer = 0; }


	bool IsAnimationFinish();








};

