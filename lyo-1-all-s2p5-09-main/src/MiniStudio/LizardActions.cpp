#include "AssetsManager.h"
#include "LizardActions.h"


void IdleLizardAction::OnStart(Lizard* pOwner)
{}

void IdleLizardAction::OnUpdate(Lizard* pOwner)
{}

void IdleLizardAction::OnEnd(Lizard* pOwner)
{}

void PatrolLizardAction::OnStart(Lizard* pOwner)
{}

void PatrolLizardAction::OnUpdate(Lizard* pOwner)
{}

void PatrolLizardAction::OnEnd(Lizard* pOwner)
{}

void AttackLizardAction::OnStart(Lizard* pOwner)
{
	pOwner->m_loopCount = 0;

	pOwner->Attack();
}

void AttackLizardAction::OnUpdate(Lizard* pOwner)
{}

void AttackLizardAction::OnEnd(Lizard* pOwner)
{}

void FleeLizardAction::OnStart(Lizard* pOwner)
{
	pOwner->frame = 0;

	pOwner->SetVelocityX(100);
}

void FleeLizardAction::OnUpdate(Lizard* pOwner)
{}

void FleeLizardAction::OnEnd(Lizard* pOwner)
{}
