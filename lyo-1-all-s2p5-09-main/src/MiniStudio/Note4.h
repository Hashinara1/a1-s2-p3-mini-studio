#pragma once
#include "Notes.h"
#include "AudioMap.h"

class Note4 : public Notes
{
private : 
	AudioMap* m_audioMap = new AudioMap();

	bool m_isHit;
	bool m_isMissed;
public : 

	void OnInitialize() override; 
	void OnFixedUpdate() override;

	void OnCollision(Entity* collidedWith);
	bool IsCollidingWithHitbar(Entity* collidedWith);
};

