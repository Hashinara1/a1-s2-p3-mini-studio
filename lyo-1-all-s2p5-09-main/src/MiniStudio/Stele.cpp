#include "Stele.h"
#include "AssetsManager.h"
#include "DialogueManager.h"

#include <string>
#include "Player.h"
void Stele::OnInitialize()
{
	mIsStatic = true;

}

void Stele::OnInteract(Player* other)
{

	other->SetRight(Player::Action::Move, false);
	other->SetRight(Player::Action::Jump, false);
	other->SetRight(Player::Action::CACatk, false);
	other->SetRight(Player::Action::Datk, false);
	other->SetRight(Player::Action::Dash, false);
	other->SetRight(Player::Action::FastFalling, false);
	other->SetRight(Player::Action::Interact, false);
	other->SetRight(Player::Action::Playing, false);
	other->SetRight(Player::Action::Teleport, false);
	other->SetRight(Player::Action::SwitchWeaponLeft, false);
	other->SetRight(Player::Action::SwitchWeaponRight, false);
	other->SetRight(Player::Action::MoveLeft, false);
	other->SetRight(Player::Action::MoveRight, false);

	std::function<void()> func = [other]() {

		other->SetRight(Player::Action::Move, true);
		other->SetRight(Player::Action::Jump, true);
		other->SetRight(Player::Action::CACatk, true);
		other->SetRight(Player::Action::Datk, true);
		other->SetRight(Player::Action::Dash, true);
		other->SetRight(Player::Action::FastFalling, true);
		other->SetRight(Player::Action::Interact, true);
		other->SetRight(Player::Action::Playing, true);
		other->SetRight(Player::Action::Teleport, true);
		other->SetRight(Player::Action::SwitchWeaponLeft, true);
		other->SetRight(Player::Action::SwitchWeaponRight, true);
		other->SetRight(Player::Action::MoveLeft, true);
		other->SetRight(Player::Action::MoveRight, true);

		};







	DialogueManager::getInstance().say(m_text.c_str(), func);
}
