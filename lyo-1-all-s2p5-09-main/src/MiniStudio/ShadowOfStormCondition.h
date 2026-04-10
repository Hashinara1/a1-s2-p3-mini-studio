#pragma once

#include "Condition.h"
#include "FirstScene.h"
#include "ShadowOfStorm.h"
#include "Utils.h"

class Player;

class IsSOSAnimationFinish : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->IsAnimationFinish();
	}
};

class IsPlayerCloseSOS : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		sf::Vector2f playerPos = s->getPlayer()->GetPosition();
		sf::Vector2f SOSPos = s->GetPosition();
		float dist = Utils::GetDistance(playerPos.x, playerPos.y, SOSPos.x, SOSPos.y);

		return dist < s->getDetectionRadius();
	}
};

class IsPlayerLessRushRange : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		sf::Vector2f playerPos = s->getPlayer()->GetPosition();
		sf::Vector2f SOSPos = s->GetPosition();
		float dist = Utils::GetDistance(playerPos.x, playerPos.y, SOSPos.x, SOSPos.y);

		return dist < s->getRushRange();
	}
};

class IsPlayerLessAttackRange : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		sf::Vector2f playerPos = s->getPlayer()->GetPosition();
		sf::Vector2f SOSPos = s->GetPosition();
		float dist = Utils::GetDistance(playerPos.x, playerPos.y, SOSPos.x, SOSPos.y);

		return dist < s->getAttackRange();
	}
};

class IsPlayerLessThunderRange : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		sf::Vector2f playerPos = s->getPlayer()->GetPosition();
		sf::Vector2f SOSPos = s->GetPosition();
		float dist = Utils::GetDistance(playerPos.x, playerPos.y, SOSPos.x, SOSPos.y);

		return dist < s->getThunderRange();
	}
};

class IsSOSFullHealth : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->isFullHealth();
	}
};

class IsSOSLesshalfHealth : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->isMoreThanHalfHealth() == false;
	}
};

class IsSOSDead : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->isDead();
	}
};


class CanSOSRush : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->canRush();
	}
};

class CanSOSHeal : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->canHeal();
	}
};

class CanSOSThunder : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->canThunder();
	}
};

class CanSOSAttack : public Condition<ShadowOfStorm> {

public:

	bool OnTest(ShadowOfStorm* s) override {
		return s->canAttack();
	}
};