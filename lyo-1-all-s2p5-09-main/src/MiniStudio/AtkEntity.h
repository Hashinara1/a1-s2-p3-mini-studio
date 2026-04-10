#pragma once

#include "Entity.h"

#include <optional>
#include <unordered_set>


class AliveEntity;


class AtkEntity : public Entity
{
private:

	int m_damage;
	float m_lifeTime;
	float m_timer;

	std::unordered_set<AliveEntity*> m_collideEntity;

public:

	void init(int _dmg, float _lt) {
		m_damage = _dmg;
		m_lifeTime = _lt;
	}

	void OnCollision(Entity* _other) override;
	void OnFixedUpdate() override;

	void addEntity(Entity* _other);
};

