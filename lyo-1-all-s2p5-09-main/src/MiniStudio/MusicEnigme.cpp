#include "MusicEnigme.h"
#include "AssetsManager.h"
#include "InputManager.h"
#include "AssetsScene.h"
#include "Player.h"

#include "Debug.h"
#include <iostream>

void MusicEnigme::OnInitialize()
{
	SetRigidBody(false);
	Player* player = nullptr;


	for (Entity* e : GameManager::Get()->getEntities()) {
		if (e->IsTag(9))
		{
			player = dynamic_cast<Player*>(e);
			break;
		}
	}

	if (player == nullptr)
		return;

	float posX = player->GetPosition().x + 10;
	float posY = player->GetPosition().y + 100;


	

	SetRigidBody(false);
	SetAffectedByFriction(false);
	SetAffectedByGravity(false);
	SetTexture(AssetsManager::getInstance().getTexture("MusicBG", "res/Ui/Background.png"));
	GetSprite().setScale(0.5f, 0.5f);
	
	
	

	m_hitline = CreateEntity<Entity>({ 150,10 }, sf::Color::Transparent);
	m_hitline->SetAffectedByGravity(false);
	m_hitline->SetRigidBody(false);
	m_hitline->SetPosition(posX, posY, 0, 0);
	
	LightSource m_bgLight;
	m_bgLight.color = sf::Color::White;
	m_bgLight.radius = 220.f;
	m_bgLight.isActive = true;
	m_bgLight.offset = { 0.5, 0.5 };
	m_hitline->setLightSource(m_bgLight);

	SetPosition(posX - 40, player->GetPosition().y - 135);
}

void MusicEnigme::OnFixedUpdate()
{
	if (isActive())
	{
		m_timer += GameManager::Get()->GetDeltaTime();
		if (IsSucceeded())
		{
			m_hasSucceeded = true;
			std::cout << "success" << std::endl;
			Destroy();

		}

		CheckInput();

		CheckFinish();


		if (m_timer >= m_delay && !CheckFinish())
		{
			m_timer = 0;
			if (m_notes.size() < 1)
				return;

			int index = m_notes.front();

			m_notes.pop_front();
			switch (index)
			{
			case 0:
				m_note1.push_back(CreateEntity<Note1>(30.f, sf::Color::Transparent));
				break;

			case 1:
				m_note2.push_back(CreateEntity<Note2>(30.f, sf::Color::Transparent));
				break;

			case 2:
				m_note3.push_back(CreateEntity<Note3>(30.f, sf::Color::Transparent));
				break;

			case 3:
				m_note4.push_back(CreateEntity<Note4>(30.f, sf::Color::Transparent));
				break;
			}
		}

	}
}


void MusicEnigme::CreateLevel(float _delay, std::list<int> _notes)
{
	m_delay = _delay;
	m_notes = _notes;
}

void MusicEnigme::CheckInput()
{
	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Sud))
	{
		if (m_note1.size() < 1)
			return;
		else if (m_note1[0]->IsCollidingWithHitbar(m_hitline))
		{
			m_note1.erase(m_note1.begin());
		}
	}

	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Est))
	{
		if (m_note2.size() < 1)
			return;
		else if (m_note2[0]->IsCollidingWithHitbar(m_hitline))
		{
			m_note2.erase(m_note2.begin());
		}
	}

	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Ouest))
	{
		if (m_note3.size() < 1)
			return;
		else if (m_note3[0]->IsCollidingWithHitbar(m_hitline))
		{
			m_note3.erase(m_note3.begin());
		}
	}

	if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Nord))
	{
		if (m_note4.size() < 1)
			return;
		else if (m_note4[0]->IsCollidingWithHitbar(m_hitline))
		{
			m_note4.erase(m_note4.begin());
		}

	}
}

bool MusicEnigme::CheckFinish()
{
	if (m_note1.size() >= 1 && m_note1[0]->GetPosition(0, 0).y > m_hitline->GetPosition(0, 1).y)
	{
		if (!ToDestroy())
		{
			Destroy();
			m_timer = 0;
			return true;
		}
	}


	else if (m_note2.size() >= 1 && m_note2[0]->GetPosition(0, 0).y > m_hitline->GetPosition(0, 1).y)
	{
		if (!ToDestroy())
		{
			Destroy();
			m_timer = 0;
			return true;
		}
	}

	else if (m_note3.size() >= 1 && m_note3[0]->GetPosition(0, 0).y > m_hitline->GetPosition(0, 1).y)
	{
		if (!ToDestroy())
		{
			Destroy();
			m_timer = 0;
			return true;
		}
	}


	else if (m_note4.size() >= 1 && m_note4[0]->GetPosition(0, 0).y > m_hitline->GetPosition(0, 1).y)
	{
		if (!ToDestroy())
		{
			Destroy();
			m_timer = 0;
			return true;
		}
	}

	return false;
}

bool MusicEnigme::IsSucceeded()
{
	if (!ToDestroy() && m_notes.empty() && m_note1.empty() && m_note2.empty() && m_note3.empty() && m_note4.empty())
		return true;

	return false;
}


void MusicEnigme::OnDestroy()
{

	m_hitline->Destroy();


	for (int i = 0; i < m_note1.size(); i++)
	{
		m_note1[i]->Destroy();
	}
	for (int i = 0; i < m_note2.size(); i++)
	{
		m_note2[i]->Destroy();
	}
	for (int i = 0; i < m_note3.size(); i++)
	{
		m_note3[i]->Destroy();
	}
	for (int i = 0; i < m_note4.size(); i++)
	{
		m_note4[i]->Destroy();
	}

	m_notes.erase(m_notes.begin(), m_notes.end());
}