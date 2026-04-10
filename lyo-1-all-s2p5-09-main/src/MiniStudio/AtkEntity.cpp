#include "AliveEntity.h"
#include "AtkEntity.h"
#include "GameManager.h"

void AtkEntity::OnFixedUpdate()
{
	m_timer += GameManager::Get()->GetDeltaTime();
	if (m_timer > m_lifeTime) {
		Destroy();
	}

	if (HasTexture())
		m_animator.update(GameManager::Get()->GetDeltaTime(), mSprite);
}

void AtkEntity::OnCollision(Entity* _other)
{


	if (AliveEntity* AE = dynamic_cast<AliveEntity*>(_other)) {
		if (m_collideEntity.count(AE) > 0)
			return;

		AE->Damage(m_damage);
		AE->OnDamage();
		m_collideEntity.insert(AE);
	}
}

void AtkEntity::addEntity(Entity* _other)
{
	// Ici un static_cast coute moins cher et le dynamic n'est pas necessaire
	AliveEntity* AE = static_cast<AliveEntity*>(_other);
	m_collideEntity.insert(AE);
}