#include "AssetsManager.h"
#include "DialogueManager.h"
#include "Player.h"
#include "PlayerActions.h"
#include "PlayerConditions.h"
#include "TransitionManager.h"
#include <iostream>


void Player::OnInitialize()
{


	m_weapons.push_back(Weapon::Attack);
	m_weapons.push_back(Weapon::Move);
	m_weapons.push_back(Weapon::Enigme);
	m_currentWeaponIndex = 0;

	SetRight(Player::Action::Jump, true);
	SetRight(Player::Action::Move, true);
	SetRight(Player::Action::FastFalling, true);
	SetRight(Player::Action::Dash, true);
	SetRight(Player::Action::CACatk, true);
	SetRight(Player::Action::Datk, true);
	SetRight(Player::Action::Playing, true);
	SetRight(Player::Action::Teleport, true);

	//all actions
	//keyboard debugging
	m_actions.bind(Action::Jump, { {sf::Keyboard::Space, TriggerState::Down} });
	m_actions.bind(Action::MoveLeft, { {sf::Keyboard::Q, TriggerState::Held} });
	m_actions.bind(Action::MoveRight, { {sf::Keyboard::D, TriggerState::Held} });
	m_actions.bind(Action::CACatk, { {sf::Mouse::Button::Left, TriggerState::Down} });
	m_actions.bind(Action::Datk, { {sf::Mouse::Button::Right, TriggerState::Down} });
	m_actions.bind(Action::FastFalling, { {sf::Keyboard::S, TriggerState::Down} });
	m_actions.bind(Action::Interact, { {sf::Keyboard::R, TriggerState::Down} });

	//weapon instrument
	m_actions.bind(Action::Datk, { {sf::Keyboard::E, TriggerState::Down} });
	m_actions.bind(Action::CACatk, { {sf::Keyboard::A, TriggerState::Down} });

	//instrument movement
	m_actions.bind(Action::Dash, { {sf::Keyboard::A, TriggerState::Down} });

	//enigmatic instrument
	m_actions.bind(Action::Playing, { {sf::Keyboard::E, TriggerState::Down} });
	m_actions.bind(Action::Dash, { {sf::Keyboard::R, TriggerState::Down} });
	m_actions.bind(Action::Interact, { {sf::Keyboard::P, TriggerState::Down} });

	//weaponSwitch
	m_actions.bind(Action::SwitchWeaponLeft, { {sf::Keyboard::Left, TriggerState::Down} });
	m_actions.bind(Action::SwitchWeaponRight, { {sf::Keyboard::Right, TriggerState::Down} });


	//Controller input
	m_actions.bind(Action::Jump, { { JoystickButton::Sud, TriggerState::Down } });
	m_actions.bind(Action::Interact, { { JoystickButton::Nord, TriggerState::Down } });

	//weapon instrument
	m_actions.bind(Action::CACatk, { {JoystickButton::Ouest, TriggerState::Down} });
	m_actions.bind(Action::Datk, { {JoystickButton::Est, TriggerState::Down} });

	//instrument movement
	m_actions.bind(Action::Teleport, { {JoystickButton::LeftTrigger, TriggerState::Down} });
	m_actions.bind(Action::Dash, { {JoystickButton::Est, TriggerState::Down} });

	//enigmatic instrument
	//m_actions.bind(Action::Playing, { {JoystickButton::Est, TriggerState::Down} });

	//weaponSwitch
	m_actions.bind(Action::SwitchWeaponRight, { {JoystickButton::R, TriggerState::Down} });
	m_actions.bind(Action::SwitchWeaponLeft, { {JoystickButton::L, TriggerState::Down} });

	//additions of light
	LightSource ls = { sf::Color::White, 275.f, {15, 0}, true };
	setLightSource(ls);


	SetUpStateMachine();

	m_hand = CreateEntity<Entity>({ 40, 40 }, sf::Color::White);
	m_hand->SetTexture(AssetsManager::getInstance().getTexture("Hand", "res/Player/Interact.png"));
	m_hand->GetSprite().setScale(3, 3);
	m_hand->SetRigidBody(false);
	m_hand->setActive(false);
	m_hand->SetAffectedByFriction(false);
	m_hand->SetAffectedByGravity(false);

	SetTag(9);

	LightSource ls2;
	ls2.color = sf::Color::White;
	ls2.isActive = true;
	ls2.offset = { 0, 0 };
	ls2.radius = 190;

	m_hand->setLightSource(ls2);
}

void Player::OnDestroy()
{
	if (m_viseur != nullptr)
	{
		m_viseur->Destroy();
	}
}

void Player::OnCollision(Entity* other)
{
	if (dynamic_cast<IInteractable*>(other) != nullptr)
		m_nearInteractable = dynamic_cast<IInteractable*>(other);

	if (other->IsStatic() && GetPosition(0, 1).y <= other->GetPosition(0, 0.1f).y)
		setGrounded(true);
}


void Player::OnDeath()
{

	if (TransitionManager::getInstance().IsFadingIn() == false && m_isDeathScreenShow == false)
	{
		TransitionManager::getInstance().SetFadeIn(true);
		TransitionManager::getInstance().SetCallBack([this]() {DeathScreen(); });
	}

	if (m_isDeathScreenShow) {


		m_deathTimer += GameManager::Get()->GetDeltaTime();

		if (m_deathTimer >= m_deathCooldown)
		{
			m_deathTimer -= 0.f;
			SetPosition(m_spawnPoint);
			Heal(getMaxHealth());
			m_isDeathScreenShow = false;
		}
	}

}

void Player::OnFixedUpdate()
{

	if (InputManager::getInstance().isKeyDown(sf::Keyboard::O))
		Damage(1);

	if (m_manaCount < 5) {
		m_manaTimer += GameManager::Get()->GetDeltaTime();
		if (m_manaTimer > m_manaInter) {
			AddManaPoint();
			m_manaTimer -= m_manaInter;
		}
	}

	m_stateMachine.Update();
	m_animator.update(GameManager::Get()->GetDeltaTime(), GetSprite());

	if (m_actions.isActive(Action::Interact)) {
		if (m_nearInteractable) {
			m_nearInteractable->OnInteract(this);
		}
	}


	isGrounded = false;
	if (m_actions.isActive(Action::SwitchWeaponLeft))
	{
		m_currentWeaponIndex--;
		if (m_currentWeaponIndex < 0)
			m_currentWeaponIndex = m_weapons.size() - 1;
	}
	if (m_actions.isActive(Action::SwitchWeaponRight))
	{
		m_currentWeaponIndex++;
		if (m_currentWeaponIndex > m_weapons.size() - 1)
			m_currentWeaponIndex = 0;
	}
	m_hand->SetPosition({ GetPosition(1, 0.5).x + 50, GetPosition(0, 0).y - 20 });

	m_hand->setActive(false);
	if (m_nearInteractable)
		m_hand->setActive(true);

	setGrounded(false);
	m_nearInteractable = nullptr;

	float VelX = GetVelocity().x;
	if (VelX < -1)
	{
		mSprite.setScale(-abs(mSprite.getScale().x), mSprite.getScale().y);
	}
	else  if (VelX > 1) {
		mSprite.setScale(abs(mSprite.getScale().x), mSprite.getScale().y);
	}

	if (isDead())
		OnDeath();

}

void Player::SetJump(int _jump)
{
	m_strenghtJump = _jump;
}

void Player::DeathScreen()
{
	m_deathTimer = 0;
	m_isDeathScreenShow = true;


}

void Player::SetUpStateMachine()
{
	//all the states of our character

	IdlePlayerAction* idle = m_stateMachine.CreateAction<IdlePlayerAction>(0);
	MovingPlayerAction* walk = m_stateMachine.CreateAction<MovingPlayerAction>(1);
	JumpingPlayerAction* jump = m_stateMachine.CreateAction<JumpingPlayerAction>(2);
	FallingPlayerAction* fall = m_stateMachine.CreateAction<FallingPlayerAction>(3);
	FastFallingPlayerAction* fastfall = m_stateMachine.CreateAction<FastFallingPlayerAction>(4);
	CACAttackingPlayerAction* CACatk = m_stateMachine.CreateAction<CACAttackingPlayerAction>(5);
	DistanceAttackPlayerAction* Datk = m_stateMachine.CreateAction<DistanceAttackPlayerAction>(6);
	DashPlayerAction* dash = m_stateMachine.CreateAction<DashPlayerAction>(7);
	PlayingPlayerAction* playing = m_stateMachine.CreateAction<PlayingPlayerAction>(8);
	HitPlayerAction* hit = m_stateMachine.CreateAction<HitPlayerAction>(9);
	TeleportPlayerAction* tp = m_stateMachine.CreateAction<TeleportPlayerAction>(10);
	m_stateMachine.SetState(0);

	//No conditional hit; we will force the transition after a Damage()

	//Idle condition
	{
		//to Walk
		auto transition = idle->CreateTransition(1);
		transition->AddCondition<IsPlayerMoving>(true);
		transition->AddCondition<CanPlayerMove>(true);

		//to Jump
		transition = idle->CreateTransition(2);
		transition->AddCondition<DoesPlayerWantJumping>(true);
		transition->AddCondition<CanPlayerJumping>(true);

		//to CACatk
		transition = idle->CreateTransition(5);
		transition->AddCondition<DoesPlayerWantCACAttacking>(true);
		transition->AddCondition<CanPlayerCACAttacking>(true);
		transition->AddCondition<DoesPlayerEquipAttackWeapon>(true);

		//to Datk
		transition = idle->CreateTransition(6);
		transition->AddCondition<doesPlayerWantDAttacking>(true);
		transition->AddCondition<CanPlayerDAttacking>(true);
		transition->AddCondition<DoesPlayerEquipAttackWeapon>(true);

		//to Dash
		transition = idle->CreateTransition(7);
		transition->AddCondition<DoesPlayerWantDashing>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerDash>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

		//to Playing
		transition = idle->CreateTransition(8);
		transition->AddCondition<DoesPlayerWantPlaying>(true);
		transition->AddCondition<CanPlayerPlaying>(true);
		transition->AddCondition<DoesPlayerEquipEnigmeWeapon>(true);

		//to Teleport
		transition = idle->CreateTransition(10);
		transition->AddCondition<DoesPlayerWantTeleport>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerTeleport>(true);

		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

		//to Fall
		idle->CreateTransition(3)->AddCondition<IsPlayerFalling>(true);

	}

	//Walk condition
	{
		//to Idle
		auto transition = walk->CreateTransition(0);
		transition->AddCondition<IsPlayerMoving>(false);


		//to Jump
		transition = walk->CreateTransition(2);
		transition->AddCondition<DoesPlayerWantJumping>(true);
		transition->AddCondition<CanPlayerJumping>(true);


		//to Fall
		walk->CreateTransition(3)->AddCondition<IsPlayerFalling>(true);

		//to Dash
		transition = walk->CreateTransition(7);
		transition->AddCondition<DoesPlayerWantDashing>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerDash>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

		//to CACatk
		transition = walk->CreateTransition(5);
		transition->AddCondition<DoesPlayerWantCACAttacking>(true);
		transition->AddCondition<CanPlayerCACAttacking>(true);
		transition->AddCondition<DoesPlayerEquipAttackWeapon>(true);

		//to Datk
		transition = walk->CreateTransition(6);
		transition->AddCondition<doesPlayerWantDAttacking>(true);
		transition->AddCondition<CanPlayerDAttacking>(true);
		transition->AddCondition<DoesPlayerEquipEnigmeWeapon>(true);


		//to Teleport
		transition = walk->CreateTransition(10);
		transition->AddCondition<DoesPlayerWantTeleport>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerTeleport>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

	}

	//Jump condition
	{
		//to Fall
		jump->CreateTransition(3)->AddCondition<IsPlayerFalling>(true);

		//to Idle
		jump->CreateTransition(0)->AddCondition<IsPlayerGrounded>(true);

		//to Dash
		auto transition = jump->CreateTransition(7);
		transition->AddCondition<DoesPlayerWantDashing>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerDash>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);


		//to Datk
		transition = jump->CreateTransition(6);
		transition->AddCondition<doesPlayerWantDAttacking>(true);
		transition->AddCondition<CanPlayerDAttacking>(true);
		transition->AddCondition<DoesPlayerEquipAttackWeapon>(true);

		//to Teleport
		transition = jump->CreateTransition(10);
		transition->AddCondition<DoesPlayerWantTeleport>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerTeleport>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

	}

	//Fall condition 
	{
		//to Idle
		fall->CreateTransition(0)->AddCondition<IsPlayerGrounded>(true);

		//to FastFall
		auto transition = fall->CreateTransition(4);
		transition->AddCondition<DoesPlayerWantFastFalling>(true);
		transition->AddCondition<CanPlayerFastFalling>(true);

		//to Dash
		transition = fall->CreateTransition(7);
		transition->AddCondition<DoesPlayerWantDashing>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerDash>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

		//to Datk
		transition = fall->CreateTransition(6);
		transition->AddCondition<doesPlayerWantDAttacking>(true);
		transition->AddCondition<CanPlayerDAttacking>(true);
		transition->AddCondition<DoesPlayerEquipAttackWeapon>(true);

		//to Teleport
		transition = fall->CreateTransition(10);
		transition->AddCondition<DoesPlayerWantTeleport>(true);
		transition->AddCondition<CanPlayerDoSpecialMove>(true);
		transition->AddCondition<CanPlayerTeleport>(true);
		transition->AddCondition<DoesPlayerEquipMoveWeapon>(true);

	}

	//FastFall condition 
	{
		//to Idle
		auto transition = fastfall->CreateTransition(0);
		transition->AddCondition<IsPlayerFastFalling>(false);
		transition->AddCondition<IsPlayerAnimationFinish>(true);

	}

	//Dash condition 
	{
		//to Idle
		auto transition = dash->CreateTransition(0);
		transition->AddCondition<IsPlayerMoving>(false);
		transition->AddCondition<IsPlayerAnimationFinish>(true);
		transition->AddCondition<IsPlayerGrounded>(true);

		//to Walk
		transition = dash->CreateTransition(1);
		transition->AddCondition<IsPlayerMoving>(true);
		transition->AddCondition<IsPlayerAnimationFinish>(true);
		transition->AddCondition<IsPlayerGrounded>(true);
		transition->AddCondition<CanPlayerMove>(true);


		//to Fall
		transition = dash->CreateTransition(3);
		transition->AddCondition<IsPlayerAnimationFinish>(true);
		transition->AddCondition<IsPlayerGrounded>(false);
	}

	//CACatk condition 
	{
		//to Idle, animation condition, to be added when we have them
		CACatk->CreateTransition(0)->AddCondition<IsPlayerAnimationFinish>(true);
	}

	//Datk condition 
	{
		//At idle, if you're on the ground, you could also be falling
		auto transition = Datk->CreateTransition(0);
		transition->AddCondition<IsPlayerAnimationFinish>(true);
		transition->AddCondition<IsPlayerGrounded>(true);

		//to Fall
		transition = Datk->CreateTransition(3);
		transition->AddCondition<IsPlayerAnimationFinish>(true);
		transition->AddCondition<IsPlayerGrounded>(false);

	}

	//Playing condition 
	{
		//to Idle, idem
		playing->CreateTransition(0)->AddCondition<HasPlayerFinishedPlaying>(true);
		playing->CreateTransition(0)->AddCondition<HasPlayerSucceededPlaying>(true);
	}

	//Hit condition 
	{
		//to Idle
		hit->CreateTransition(0)->AddCondition<IsPlayerAnimationFinish>(true);
	}

	//Teleport condition 
	{
		tp->CreateTransition(0)->AddCondition<DoesPlayerWantTeleport>(false);
	}

}

sf::Vector2f Player::GetViseurPos()
{

	if (m_stateMachine.GetCurrentState() == 10)
		return m_viseur->GetPosition();
	return sf::Vector2f();

}

void Player::Jump()
{
	SetVelocityY(m_strenghtJump);
}

void Player::ApplyAirMovement()
{

	if (getRight(Player::Action::Move) == false)
		return;

	if (abs(InputManager::getInstance().getAxisValue(JoystickAxis::LeftJoystickX)) > 10)
		MoveX();

	if (m_actions.isActive(Action::MoveLeft))
		MoveLeft();


	if (m_actions.isActive(Action::MoveRight))
		MoveRight();



}

void Player::MoveX()
{
	float VelX = InputManager::getInstance().getAxisValue(JoystickAxis::LeftJoystickX);
	AddVelocity(sf::Vector2f(mSpeed * (VelX / 100), 0));
}

void Player::MoveRight()
{
	AddVelocity(sf::Vector2f(mSpeed * 100, 0));
}

void Player::MoveLeft()
{
	AddVelocity(sf::Vector2f(mSpeed * -100, 0));
}

void Player::AddManaPoint()
{
	std::min(m_manaCount++, 6);

}

