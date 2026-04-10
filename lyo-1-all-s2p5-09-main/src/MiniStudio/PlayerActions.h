#pragma once
#include "Action.h"
class Player;
class AssetsScene;

#include <SFML/System/Vector2.hpp>

class Entity;
class Player;

class IdlePlayerAction : public Action<Player>
{
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override ;
	void OnEnd(Player* pOwner) override {};
};

class MovingPlayerAction : public Action<Player>
{
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override {};
};

class JumpingPlayerAction : public Action<Player>
{
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};

class FallingPlayerAction : public Action<Player> {

	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};

class FastFallingPlayerAction : public Action<Player> {

	float m_lastMaxFallSpeed;

	
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};

class HitPlayerAction : public Action<Player> {


	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override {};
	void OnEnd(Player* pOwner) override {};
};

class CACAttackingPlayerAction : public Action<Player>
{
	
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};

class DistanceAttackPlayerAction: public Action<Player>{

	
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};

class DashPlayerAction : public Action<Player> {

	float m_lastMaxRunSpeed;
	
	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override {};
	void OnEnd(Player* pOwner) override;
};

class PlayingPlayerAction : public Action<Player> {

	Scene* scene ;

	float m_animationDuration = 2.f;
	float m_timer = 0.f;

	// Herite via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override ;
};

class TeleportPlayerAction : public Action<Player> {

	float m_limitRadius = 450.f;
	sf::Vector2f m_delta = { 0, 0 };

	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;

public:

	sf::Vector2f getDelta() { return m_delta; }
};


class StunPlayerAction : public Action<Player> {

	// Inherited via Action
	void OnStart(Player* pOwner) override;
	void OnUpdate(Player* pOwner) override;
	void OnEnd(Player* pOwner) override;
};