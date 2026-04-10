#ifndef PAUSE_UI_H__
#define PAUSE_UI_H__

#include "GameManager.h"
#include "UI.h"

enum InputAction
{
	Comfirm,
	Left,
	Right,
	Up,
	Down
};
class PauseUI : public UI
{
private:
	Bouton* target;
	float mAccumulatedDeltaTime;
	float m_inter;
	InputManager* inputManager;
	Bouton* nearest;
	float dist;

	bool start;
	bool end;

	float decPosOut;
	float decPosCurrent;

	Entity* light;
	std::shared_ptr<Entity> Cursor;

protected:
	PauseUI();
	~PauseUI();

	void OnInit() override;
	void FixedUpdate() override;
	void OnWindowEvent() override;
	void SetActif(bool it) override;

	void UpdateInput(InputAction);

public:
	static PauseUI& Get();
};


#endif // !PAUSE_UI_H__