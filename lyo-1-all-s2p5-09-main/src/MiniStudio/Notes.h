#pragma once
#include "Entity.h"


class Notes : public Entity
{
private:
	int m_velocity;
public:
	int notesCount = 0;
	int lane;
	void OnInitialize() override; 
	void OnFixedUpdate()override;
};

