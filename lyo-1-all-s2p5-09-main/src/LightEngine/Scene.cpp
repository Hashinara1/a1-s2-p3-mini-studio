#include "Debug.h"
#include "GameManager.h"
#include "Scene.h"

int Scene::GetWindowWidth() const
{
	return mpGameManager->mWindowWidth;
}

int Scene::GetWindowHeight() const
{
	return mpGameManager->mWindowHeight;
}



float Scene::GetDeltaTime() const
{
	return mpGameManager->mDeltaTime;
}

