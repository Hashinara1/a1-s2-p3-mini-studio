#pragma once

#include "Condition.h"
#include "AssetsScene.h"
#include "MusicEnigme.h"
#include "Player.h"

class IsPlayerMoving : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return abs(p->GetVelocity().x) > 10;
	}
};

class DoesPlayerWantMoving : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		
		bool cond = false;

		cond |= p->GetActionMap().isActive(Player::Action::MoveLeft); //keyboard debugging
		cond |= p->GetActionMap().isActive(Player::Action::MoveRight); // keyboard debugging

		cond |= abs(InputManager::getInstance().getAxisValue(JoystickAxis::LeftJoystickX)) > 10;
		return cond;
	}

};

class DoesPlayerEquipAttackWeapon : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getCurentWeapon() == 0;
	}
};

class DoesPlayerEquipMoveWeapon : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getCurentWeapon() == 1;
	}
};

class DoesPlayerEquipEnigmeWeapon : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getCurentWeapon() == 2;
	}
};

class CanPlayerMove : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Move);
	}
};

class DoesPlayerWantJumping : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::Jump);
	}
};

class CanPlayerJumping : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Jump);
	}
};

class IsPlayerFalling : public Condition<Player> {

	bool OnTest(Player* p) override {
		return !p->isGrounded && p->GetVelocity().y > 0;
	}
};

class IsPlayerJumping : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return !p->isGrounded && p->GetVelocity().y < 0;
	}

};

class IsPlayerGrounded : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->isGrounded;
	}
};

class IsPlayerFastFalling : public Condition<Player> {

	bool OnTest(Player* p) override {
		return p->GetVelocity().y > GameManager::Get()->GetScene()->GetGravity().y * 2;
	}
};

class DoesPlayerWantFastFalling : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::FastFalling);
	}
};

class CanPlayerFastFalling : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::FastFalling);
	}
};

class DoesPlayerWantDashing : public Condition<Player> {

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::Dash);
	}
};

class CanPlayerDash : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Dash);
	}
};

class DoesPlayerWantCACAttacking : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::CACatk);
	}
};

class CanPlayerCACAttacking : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::CACatk);
	}
};

class doesPlayerWantDAttacking : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::Datk) && p->getRight(Player::Action::Datk);
	}
};

class CanPlayerDAttacking : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Datk);
	}
};

class DoesPlayerWantPlaying : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::Playing) ;
	}
};

class HasPlayerFinishedPlaying : public Condition<Player> {
	public:
	bool OnTest(Player* p) override {
		Scene* scene = p->GetScene();
		
		return scene->enigme1->CheckFinish();
	}
};

class HasPlayerSucceededPlaying : public Condition<Player> {
public:
	bool OnTest(Player* p) override {
		Scene* scene = p->GetScene();

		return scene->enigme1->IsSucceeded();
	}
};

class CanPlayerPlaying : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Playing);
	}
};

class IsPlayerAnimationFinish : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetAnimator().isFinished();
	}
};

class DoesPlayerWantTeleport : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->GetActionMap().isActive(Player::Action::Teleport);
	}
};

class CanPlayerTeleport : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getRight(Player::Action::Teleport);
	}
};

class CanPlayerDoSpecialMove : public Condition<Player> {

public:

	bool OnTest(Player* p) override {
		return p->getManaCount() > 0;
	}
};