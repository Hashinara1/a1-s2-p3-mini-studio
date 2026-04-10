#pragma once

class Player;
class IInteractable {
public:

	virtual void OnInteract(Player* other) = 0;
};