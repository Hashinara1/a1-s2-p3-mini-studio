#include "Notes.h"
#include "Debug.h"
#include "random"
void Notes::OnInitialize()
{
	m_velocity = 90 + rand() % 30;
	SetVelocityY(0);
	SetAffectedByGravity(false);
	SetAffectedByFriction(false);
	SetRigidBody(false);
	
	
}

void Notes::OnFixedUpdate()
{
	if (isActive())
	{
		SetVelocityY(m_velocity);
	}
}
