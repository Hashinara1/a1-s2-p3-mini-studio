#pragma once

#include "AliveEntity.h"
#include "ActionMap.h"
#include "IInteractable.h"
#include "StateMachine.h"

#include <unordered_map>


class Item;

enum class Weapon {

	Attack = 0,
	Move = 1,
	Enigme = 2,
};

class Player : public AliveEntity
{
public:

	bool isGrounded = false;

	enum class Action {
		Jump,
		Move,
		MoveRight,
		MoveLeft,
		FastFalling,
		Dash,
		CACatk,
		Datk,
		Playing,
		Teleport,
		Interact,
		SwitchWeaponLeft,
		SwitchWeaponRight,
	};


	void setGrounded(bool _state) { isGrounded = _state; }

	void SetRight(Action _action, bool _state) { m_right[_action] = _state; }
	void setSpawnPoint(sf::Vector2f _spawnPoint) { m_spawnPoint = _spawnPoint; }

	int getCurentWeapon() { return m_currentWeaponIndex; }
	bool getDeathScreenShow() { return m_isDeathScreenShow; }
	int getManaCount() { return m_manaCount; }
	bool getRight(Action _action) { return m_right[_action];}
	sf::Vector2f getSpawnPoint() { return m_spawnPoint; }

	const ActionMap<Action>& GetActionMap() {
		return m_actions;
	}

	sf::Vector2f GetViseurPos();
	StateMachine<Player>& GetStateMachine() { return  m_stateMachine; }

	void init(int _maxHp) override { AliveEntity::init(_maxHp); }

	void OnCollision(Entity* other) override;
	void OnDeath();
	void OnDestroy() override;
	void OnFixedUpdate() override;
	void OnInitialize() override;


	void Jump();
	void ApplyAirMovement();
	void MoveX();
	void MoveRight();
	void MoveLeft();

	void RemoveManaPoint() { m_manaCount--; }
	void AddManaPoint();
	

	int money = 0;

	void SetJump(int _jump);

private:
	
	void DeathScreen();
	void SetUpStateMachine();

	sf::Vector2f m_spawnPoint;

	std::unordered_map<Action, bool> m_right;

	Entity* m_viseur = nullptr;
	
	Entity* m_hand;
	bool m_isDeathScreenShow = false;
	IInteractable* m_nearInteractable;


	std::vector<Weapon> m_weapons;
	int m_currentWeaponIndex;


	int m_CACdamage = 5;
	int m_Ddamage = 3;
	int m_manaCount = 5;

	int m_strenghtJump = -800;

	float m_deathCooldown = 3.f;
	float m_deathTimer = 0.f;
	float m_manaInter = 2.5f;
	float m_manaTimer;


	StateMachine<Player> m_stateMachine = StateMachine<Player>(this, 12);

	ActionMap<Action> m_actions;

	friend class CACAttackingPlayerAction;
	friend class DistanceAttackPlayerAction;
	friend class TeleportPlayerAction;
};

