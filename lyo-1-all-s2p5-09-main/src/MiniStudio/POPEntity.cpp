#include "POPEntity.h"

void POPEntity::OnFixedUpdate()
{
	m_timer += GameManager::Get()->GetDeltaTime();
	if (m_timer > m_lifeTime) {
		Destroy();
	}

	LightSource ls = getLightSource().value();

	ls.color.a += m_alphaDelta * GameManager::Get()->GetDeltaTime();

	setLightSource(ls);

	if (ls.color.a <= 128) {
		m_alphaDelta *= -1;
		m_alphaDelta += 3;
	}

	if (ls.color.a >= 255) {
		m_alphaDelta *= -1;
		m_alphaDelta -= 3;
	}
}
