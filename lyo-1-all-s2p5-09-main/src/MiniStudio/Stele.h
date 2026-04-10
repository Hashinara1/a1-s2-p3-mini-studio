#pragma once
#include "Entity.h"
#include "IInteractable.h"

class Stele : public Entity, public IInteractable
{
private:

	std::string m_text;

public:

	void setText(std::string _text) { m_text = _text; }

	void OnInitialize() override;

	void OnInteract(Player* other) override;

};

