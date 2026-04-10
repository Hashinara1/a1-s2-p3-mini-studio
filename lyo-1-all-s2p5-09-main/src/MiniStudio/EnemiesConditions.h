#pragma once
#include "Condition.h"
#include "Enemies.h"
#include "Utils.h"


class Enemie1IsPlayerFront : public Condition<Enemie1>
{
public:
	bool OnTest(Enemie1* e) override {
		sf::Vector2f playerPos = e->getPlayer()->GetPosition();
		sf::Vector2f ePos = e->GetPosition();

		int dir = e->GetSprite().getScale().x < 0 ? -1 : 1;
		float dist = Utils::GetDistance(playerPos, ePos);

		if (dist > 1500)
			return false;


		if (dir > 0 && playerPos.x < ePos.x)
			return true;
		if (dir < 0 && playerPos.x > ePos.x)
			return true;

		return false;
	}
};


class Enemie1CanAttack : public Condition<Enemie1> {
public:
	bool OnTest(Enemie1* e) override {
		
		return e->CanAttack();
	}
};

class Enemie2IsPlayerFront : public Condition<Enemie2>
{
public:
	bool OnTest(Enemie2* e) override {
		sf::Vector2f playerPos = e->getPlayer()->GetPosition();
		sf::Vector2f ePos = e->GetPosition();

		int dir = e->GetSprite().getScale().x < 0 ? -1 : 1;
		float dist = Utils::GetDistance(playerPos, ePos);

		if (dist > 80)
			return false;
		//if (dir < 0 && playerPos.x < ePos.x)
		//	return true;
		//if (dir > 0 && playerPos.x > ePos.x)
		//	return true;

		return true;
	}
};


class Enemie2CanAttack : public Condition<Enemie2> {
public:
	bool OnTest(Enemie2* e) override {
		return e->CanAttack();
	}
};

class Enemie3IsPlayerFront : public Condition<Enemie3>
{

public:
	bool OnTest(Enemie3* e) override {
		sf::Vector2f playerPos = e->getPlayer()->GetPosition();
		sf::Vector2f ePos = e->GetPosition();

		int dir = e->GetSprite().getScale().x < 0 ? -1 : 1;
		float dist = Utils::GetDistance(playerPos, ePos);

		if (dist > 120)
			return false;


		return true;
	}
};


class Enemie3CanAttack : public Condition<Enemie3> {
public:
	bool OnTest(Enemie3* e) override {
		return e->CanAttack();
	}
};