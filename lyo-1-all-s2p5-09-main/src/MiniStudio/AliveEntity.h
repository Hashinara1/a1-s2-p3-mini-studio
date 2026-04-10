#pragma once

#include "Entity.h"
class AliveEntity : public Entity
{
protected:
	int m_maxHp;
	int m_hp;

public:

	virtual ~AliveEntity() = default;

	virtual void init(int _mh)
	{
		m_maxHp = _mh;
		m_hp = _mh;
	}

	void virtual Damage(int _amout) {
		m_hp -= _amout;
		OnDamage();
		if (m_hp < 0)
			OnDead();
	}

	bool isDead() const {
		return (m_hp <= 0);
	}
	
	bool isFullHealth() const {
		return m_hp == m_maxHp;
	}

	bool isMoreThanHalfHealth() const {
		return m_hp >= m_maxHp / 2;
	}

	void Heal(int _amout) {
		m_hp = m_hp + _amout > m_maxHp ? m_maxHp : m_hp + _amout;
	}

	
	virtual void OnDamage() {};
	virtual void OnDead() {};


	int getCurrentHealth() const { return m_hp; }
	int getMaxHealth() const { return m_maxHp; }

	virtual void OnDestroy() override {};
};

