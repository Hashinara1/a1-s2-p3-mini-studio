#include "AudioManager.h"
#include "AssetsManager.h"
#include "AtkEntity.h"
#include "AssetsScene.h"
#include "MusicEnigme.h"

#include "Utils.h"

#include "Debug.h"
#include "Define.h"
#include "GameManager.h"
#include "PlayerActions.h"
#include "Player.h"
#include "Utils.h"


void MovingPlayerAction::OnStart(Player* pOwner)
{
	pOwner->GetAnimator().play("Run");

}

void MovingPlayerAction::OnUpdate(Player* pOwner)
{
	pOwner->ApplyAirMovement();
	uint64_t delay = rand() % 1000;
	pOwner->GetScene()->getAudioMap()->playSound("FootStep", delay);

}


void JumpingPlayerAction::OnStart(Player* pOwner)
{
	pOwner->Jump();
	pOwner->GetAnimator().play("Jump");
}

void JumpingPlayerAction::OnUpdate(Player* pOwner)
{

	pOwner->ApplyAirMovement();

}

void JumpingPlayerAction::OnEnd(Player* pOwner)
{}


void CACAttackingPlayerAction::OnStart(Player* pOwner)
{


	pOwner->SetVelocityX(0);
	pOwner->GetAnimator().play("CACAttack");

}

void CACAttackingPlayerAction::OnUpdate(Player* pOwner)
{
}

void CACAttackingPlayerAction::OnEnd(Player* pOwner)
{

	AtkEntity* atk = pOwner->CreateEntity<AtkEntity>({ 32, 32 }, sf::Color::Red);

	atk->SetRigidBody(false);
	atk->SetStatic(false);
	atk->SetAffectedByGravity(false);
	int dir = pOwner->GetSprite().getScale().x < 0 ? -1 : 1;

	sf::Vector2f playerpos = pOwner->GetPosition();
	atk->SetPosition(playerpos.x + 24 * dir, playerpos.y);
	atk->init(3, 0.6f);
	atk->SetTexture(AssetsManager::getInstance().getTexture("BasicSwordEffect", "res/Player/AttaqueCaC_PersoPrincipal.png"));
	atk->GetAnimator().loadFromJson("res/JsonAnimation/CACAttackPlayer.json");
	atk->GetAnimator().applyScale(atk->GetSprite(), { 250.f, 250.f });
	atk->SetSpriteOffsetFlipped(sf::Vector2f(-340, -150));
	atk->SetSpriteOffset(sf::Vector2f(-150, -150));
	atk->GetAnimator().play("atk");
	atk->addEntity(pOwner);
	if (dir == 1) {
		atk->GetSprite().setOrigin(atk->GetSprite().getLocalBounds().width, 0.f);
		atk->GetSprite().setScale(-abs(atk->GetSprite().getScale().x), atk->GetSprite().getScale().y);
	}
}


void FallingPlayerAction::OnStart(Player* pOwner)
{
	pOwner->GetAnimator().play("Fall");
}

void FallingPlayerAction::OnUpdate(Player* pOwner)
{
	pOwner->ApplyAirMovement();
}

void FallingPlayerAction::OnEnd(Player* pOwner)
{
	pOwner->GetAnimator().play("Stun");
}


void FastFallingPlayerAction::OnStart(Player* pOwner)
{
	m_lastMaxFallSpeed = pOwner->GetMaxFallSpeed();
	pOwner->SetMaxFallSpeed(m_lastMaxFallSpeed * 2);
}

void FastFallingPlayerAction::OnUpdate(Player* pOwner)
{
	pOwner->AddVelocity(GameManager::Get()->GetScene()->GetGravity());
}

void FastFallingPlayerAction::OnEnd(Player* pOwner)
{
	pOwner->SetMaxFallSpeed(m_lastMaxFallSpeed);
}

void HitPlayerAction::OnStart(Player* pOwner)
{
	pOwner->GetAnimator().play("Hit");
}

void DistanceAttackPlayerAction::OnStart(Player* pOwner)
{


	pOwner->GetAnimator().play("DAttack");

}

void DistanceAttackPlayerAction::OnUpdate(Player* pOwner)
{
	pOwner->ApplyAirMovement();
}

void DistanceAttackPlayerAction::OnEnd(Player* pOwner)
{

	if (pOwner->getManaCount() == 0)
		return;
	pOwner->RemoveManaPoint();
	AtkEntity* atk = pOwner->CreateEntity<AtkEntity>({ 180, 180 }, sf::Color::Red);

	atk->SetRigidBody(false);
	atk->SetStatic(false);
	atk->SetAffectedByGravity(false);
	atk->SetAffectedByFriction(false);
	int dir = pOwner->GetSprite().getScale().x < 0 ? -1 : 1;
	atk->SetVelocityX(dir * 300);
	atk->SetMaxRunSpeed(999);
	sf::Vector2f playerpos = pOwner->GetPosition();
	atk->SetPosition(playerpos.x + 16 * dir, playerpos.y);
	atk->init(1, 3);
	atk->SetTexture(AssetsManager::getInstance().getTexture("CACatk", "res/Player/AttaqueDistance_Perso_Principal.png"));
	atk->GetAnimator().loadFromJson("res/JsonAnimation/DistanceAttackAnimation.json");
	atk->GetAnimator().play("DistanceAttackAnimation");
	atk->GetAnimator().applyScale(atk->GetSprite(), sf::Vector2f(250, 250));
	atk->SetSpriteOffsetFlipped(sf::Vector2f(-350, -50));
	atk->SetSpriteOffset(sf::Vector2f(0, -50));

	if (dir == 1) {
		atk->GetSprite().setOrigin(atk->GetSprite().getLocalBounds().width, 0.f);
		atk->GetSprite().setScale(-abs(atk->GetSprite().getScale().x), atk->GetSprite().getScale().y);
	}

	atk->addEntity(pOwner);


}


void DashPlayerAction::OnStart(Player* pOwner)
{
	m_lastMaxRunSpeed = pOwner->GetMaxRunSpeed();
	pOwner->SetMaxRunSpeed(m_lastMaxRunSpeed * 3);

	int facing = pOwner->GetSprite().getScale().x < 0 ? -1 : 1;
	pOwner->SetVelocityX(facing * pOwner->GetMaxRunSpeed());
	pOwner->RemoveManaPoint();
	pOwner->GetAnimator().play("Dash");
	pOwner->GetScene()->getAudioMap()->playSound("Dash");
}

void DashPlayerAction::OnEnd(Player* pOwner)
{
	pOwner->SetMaxRunSpeed(m_lastMaxRunSpeed);
}

void PlayingPlayerAction::OnStart(Player* pOwner)
{
	m_timer = 0;
	
	scene = pOwner->GetScene();

	scene->enigme1 = pOwner->CreateEntity<MusicEnigme>({ 20,20 }, sf::Color::White);
	scene->enigme1->CreateLevel(0.5f, std::list<int>{0, 1, 2, 3});
	scene->enigme1->setActive(true);
	pOwner->GetScene()->getCam().get()->zoom(0.6f);


	pOwner->SetRight(Player::Action::Move, false);
	pOwner->SetRight(Player::Action::Jump, false);
	pOwner->SetRight(Player::Action::FastFalling, false);
	pOwner->SetRight(Player::Action::Dash, false);
	pOwner->SetRight(Player::Action::CACatk, false);
	pOwner->SetRight(Player::Action::Datk, false);
	pOwner->SetRight(Player::Action::Teleport, false);
}

void PlayingPlayerAction::OnUpdate(Player* pOwner)
{

}

void PlayingPlayerAction::OnEnd(Player* pOwner)
{
	pOwner->SetRight(Player::Action::Move, true);
	pOwner->SetRight(Player::Action::Jump, true);
	pOwner->SetRight(Player::Action::FastFalling, true);
	pOwner->SetRight(Player::Action::Dash, true);
	pOwner->SetRight(Player::Action::CACatk, true);
	pOwner->SetRight(Player::Action::Datk, true);
	pOwner->SetRight(Player::Action::Teleport, true);

	pOwner->GetScene()->getCam().get()->zoom(1.65f);
}

void TeleportPlayerAction::OnStart(Player* pOwner)
{
	if (pOwner->m_viseur == nullptr) {
		pOwner->m_viseur = pOwner->CreateEntity<Entity>(50, sf::Color::Red);
		pOwner->m_viseur->SetRigidBody(false);
		pOwner->m_viseur->SetAffectedByGravity(false);
		pOwner->m_viseur->SetAffectedByFriction(false);
	}
	pOwner->m_viseur->setActive(true);
	pOwner->m_viseur->SetPosition(pOwner->GetPosition().x, pOwner->GetPosition().y);
	m_delta = { 0.1,-0.1 };
}

void TeleportPlayerAction::OnUpdate(Player* pOwner)
{
	InputManager& im = InputManager::getInstance();

	m_delta.x += im.getAxisValue(JoystickAxis::RightJoystickX) / 3;
	m_delta.y += im.getAxisValue(JoystickAxis::RightJoystickY) / 3;
	if (m_delta.x != 0 && m_delta.y != 0)
	{
		float dist = Utils::GetDistance(m_delta.x, m_delta.y, 0, 0);
		if (dist > m_limitRadius)
		{
			m_delta /= dist;
			m_delta *= m_limitRadius;
		}
		sf::Vector2f newPos = pOwner->GetPosition() + m_delta;
		pOwner->m_viseur->SetPosition(newPos.x, newPos.y);
	}


}

void TeleportPlayerAction::OnEnd(Player* pOwner)
{
	pOwner->m_viseur->setActive(false);

	bool isColliding = false;
	bool isCollidingWithPlatform = false;

	if (Utils::GetDistance(pOwner->GetPosition(), pOwner->m_viseur->GetPosition()) < 15)
		return;
	pOwner->SetPosition(pOwner->m_viseur->GetPosition().x, pOwner->m_viseur->GetPosition().y);


	Entity* collidingEntityPlatform = nullptr;
	for (Entity* it : GameManager::Get()->getEntities())
	{
		if (!it->IsRigidBody() || it == pOwner)
			continue;
		if (pOwner->IsColliding(it))
		{
			if (it->GetIsPlatform())
			{
				isCollidingWithPlatform = true;
				collidingEntityPlatform = it;
			}
			isColliding = true;
			break;
		}
	}

	if (isColliding)
	{
		bool foundFreeSpace = false;

		float dist = Utils::GetDistance(m_delta.x, m_delta.y, 0, 0);
		sf::Vector2f tmp = m_delta / dist;


		while (isCollidingWithPlatform)
		{
			pOwner->SetPosition(pOwner->GetPosition() + sf::Vector2f{ 0,-1 });
			if (!pOwner->IsColliding(collidingEntityPlatform))
			{
				isCollidingWithPlatform = false;

				// Make sure that there is no collision on top of the platform
				for (Entity* it : GameManager::Get()->getEntities())
				{
					isColliding = false;
					if (!it->IsRigidBody() || it == pOwner)
						continue;
					if (pOwner->IsColliding(it))
					{
						isColliding = true;
						break;
					}
				}
			}
		}

		int it = 1;

		while (isColliding && !isCollidingWithPlatform)
		{
			for (int i = 0; i < 8; i++)
			{
				float x = tmp.x;
				float y = tmp.y;
				tmp.x = x * std::cos(22.5) - y * std::sin(22.5);
				tmp.y = x * std::sin(22.5) + y * std::cos(22.5);
				pOwner->SetPosition(pOwner->m_viseur->GetPosition() + tmp * (float)it);

				for (Entity* entity : GameManager::Get()->getEntities())
				{
					isColliding = false;
					if (!entity->IsRigidBody() || entity == pOwner)
						continue;
					if (pOwner->IsColliding(entity))
					{
						isColliding = true;
						break;
					}
				}
				if (isColliding == false)
					break;
			}
			it++;
		}
	}
	pOwner->RemoveManaPoint();
}

void IdlePlayerAction::OnStart(Player* pOwner)
{
	pOwner->GetAnimator().play("Idle");
}

void IdlePlayerAction::OnUpdate(Player* pOwner)
{
	if (pOwner->getRight(Player::Action::Move) == false)
		return;

	if (abs(InputManager::getInstance().getAxisValue(JoystickAxis::LeftJoystickX)) > 10)
		pOwner->MoveX();

	ActionMap<Player::Action> am = pOwner->GetActionMap();
	if (am.isActive(Player::Action::MoveLeft))
		pOwner->MoveLeft();
	if (am.isActive(Player::Action::MoveRight))
		pOwner->MoveRight();
}

void StunPlayerAction::OnStart(Player* pOwner)
{
	pOwner->GetAnimator().play("Stun");
	pOwner->SetVelocityX(0);
}

void StunPlayerAction::OnUpdate(Player* pOwner)
{}

void StunPlayerAction::OnEnd(Player* pOwner)
{}
