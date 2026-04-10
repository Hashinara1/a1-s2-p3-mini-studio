#pragma once

#include "Entity.h"

class POPEntity : public Entity
{
private:

	float m_alphaDelta = -200;
	float m_lifeTime = -1;
	float m_timer = 0;

public:

	void OnFixedUpdate() override;

	void init(float _lt, LightSource _ls) { m_lifeTime = _lt; setLightSource(_ls); }

};

