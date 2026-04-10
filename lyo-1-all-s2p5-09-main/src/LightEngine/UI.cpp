#include "UI.h"
#include "GameManager.h"
#include "Debug.h"

UI::UI() : actif(false), mpGameManager(GameManager::Get()) {};

UI::~UI()
{
	for (Bouton* it : m_boutons)
	{
		delete it;
	}
	m_entities.clear();
}

void UI::SetActif(bool it)
{
	actif = it;
}

bool UI::isActif()
{
	return actif;
}
int UI::GetWindowWidth() const
{
	return mpGameManager->mWindowWidth;
}

int UI::GetWindowHeight() const
{
	return mpGameManager->mWindowHeight;
}

float UI::GetDeltaTime() const
{
	return mpGameManager->mDeltaTime;
}

Bouton* UI::CreateBouton(std::shared_ptr<Entity> entity, BoutonActionBase* boutonAction)
{
	Bouton* result = new Bouton{ entity,boutonAction };
	m_boutons.push_back(result);
	return result;
}


void UI::Render()
{
	for (std::shared_ptr<Entity> it : m_entities)
	{
		if (it.get()->HasTexture())
		{
			mpGameManager->GetWindow()->draw(it.get()->GetSprite());
		}
		//Debug::DrawRectangle(it->GetPosition(0, 0).x, it->GetPosition(0, 0).y, it->GetSize().x, it->GetSize().y, sf::Color::Blue);

	}
}