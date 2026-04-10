#pragma once
#include "Entity.h"
#include "Note1.h"
#include "Note2.h"
#include "Note3.h"
#include "Note4.h"

class MusicEnigme : public Entity
{
private : 
	int m_keynumbers;
	int m_maxNotes;
	float m_timer = 0;
	float m_delay;

	bool m_hasSucceeded;
	std::list<int> m_notes;

	Entity* m_hitline;
	
	std::vector<Note1*> m_note1;
	std::vector<Note2*> m_note2;
	std::vector<Note3*> m_note3;
	std::vector<Note4*> m_note4;

	sf::Sprite* m_background;
	

public :

	void OnInitialize() override;
	void OnFixedUpdate() override;

	void CreateLevel( float _delay, std::list<int> _notes);
	void CheckInput();
	void OnDestroy() override;
	bool CheckFinish();
	bool IsSucceeded();
};

