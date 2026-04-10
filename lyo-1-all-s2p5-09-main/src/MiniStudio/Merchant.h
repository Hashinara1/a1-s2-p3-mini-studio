#pragma once

#include "Entity.h"

#include <unordered_map>
#include <string>

class Weapon;

class Item;

class Merchant : public Entity
{
private:

	std::unordered_map<Item*, int> m_items;
	std::unordered_map<Weapon, int> m_weapon;

public:

	void ShowArticle();
	void OnInitialize() override;
};

