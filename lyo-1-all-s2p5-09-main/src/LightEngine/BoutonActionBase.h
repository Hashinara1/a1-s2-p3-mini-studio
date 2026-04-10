#ifndef BOUTON_ACTION_BASE_H__
#define BOUTON_ACTION_BASE_H__

class UI;

class BoutonActionBase
{
protected:
	UI* m_ui;
public:
	void SetMyParentUI(UI* it) {
		m_ui = it;
	};
	virtual void Action() = 0;
};

#endif // !BOUTON_ACTION_BASE_H__=