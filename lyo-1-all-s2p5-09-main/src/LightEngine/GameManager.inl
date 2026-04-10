#pragma once

#include "GameManager.h"
#include "Scene.h"
#include "Entity.h"

template<typename T>
void GameManager::LaunchScene()
{
	static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
	mpNextScene = new T();
}
