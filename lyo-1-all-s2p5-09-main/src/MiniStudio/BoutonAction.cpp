#include "BoutonAction.h"
#include "GameManager.h"
#include <cstdlib>
#include "UI.h"

void BoutonActionContinuePauseMenu::Action()
{
	m_ui->SetActif(false);
	GameManager::Get()->SetPause(false);
}

void BoutonActionExitPauseMenu::Action()
{
	std::exit(0);
}