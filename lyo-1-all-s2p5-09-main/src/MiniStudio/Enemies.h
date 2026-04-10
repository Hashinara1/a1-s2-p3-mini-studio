#pragma once
#include "AliveEntity.h"
#include "StateMachine.h"

class Enemie1 : public AliveEntity
{
private:
	float m_attackInter = 1.f;
	float m_timer = 0;
	Entity* m_player;
	
	StateMachine<Enemie1> m_stateMachine = StateMachine<Enemie1>(this, 3);
	

	void setUpStateMachine();

public:


	void resetTimer() { m_timer = 0.f; }
	bool CanAttack() const { return m_timer >= m_attackInter; }

	Entity* getPlayer() const { return m_player; }


	void OnDestroy() override;
	void OnFixedUpdate() override;
	void OnInitialize() override;

	void OnDead() override;
};

class Enemie2 : public AliveEntity
{
private:
	float m_attackInter = 1.8f;
	float m_timer = 0;
	Entity* m_player;


	StateMachine<Enemie2> m_stateMachine = StateMachine<Enemie2>(this, 3);


	void setUpStateMachine();

public:
	void ResetTimer() { m_timer = 0; }

	bool CanAttack() const { return m_timer >= m_attackInter; }

	Entity* getPlayer() const { return m_player; }

	void OnDestroy() override {};
	void OnFixedUpdate() override;
	void OnInitialize() override;


	void OnDead() override;

};

class Enemie3 : public AliveEntity
{
private:
	float m_attackInter = 3.6f;
	float m_timer = 0;
	Entity* m_player;


	StateMachine<Enemie3> m_stateMachine = StateMachine<Enemie3>(this, 3);

	void setUpStateMachine();
public:

	void ResetTimer() { m_timer = 0; }

	bool CanAttack() const { return m_timer >= m_attackInter; }

	Entity* getPlayer() const { return m_player; }

	void OnDestroy() override {};
	void OnFixedUpdate() override;
	void OnInitialize() override;


	void OnDead() override;

};
