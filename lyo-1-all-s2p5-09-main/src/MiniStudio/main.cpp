#include "ActionMap.h"
#include "AnimationScene.h"
#include "LvL1Scene.h"
#include "ActionMap.h"

#include <crtdbg.h>
#include "AssetsScene.h"


int main()
{
	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "Gostelium", "../../../res/Ui/LogoScreen/Logo.png", 60, sf::Color(128, 128, 128));

	pInstance->LaunchScene<LvL1Scene>();


	pInstance->SetUp();
	return 0;
}
