#ifndef BOUTON_ACTION_H__
#define BOUTON_ACTION_H__

#include "BoutonActionBase.h"

class BoutonActionContinuePauseMenu:public BoutonActionBase
{
public:
	void Action() override;
};
class BoutonActionExitPauseMenu :public BoutonActionBase
{
public:
	void Action() override;
};
#endif // !BOUTON_ACTION_H__