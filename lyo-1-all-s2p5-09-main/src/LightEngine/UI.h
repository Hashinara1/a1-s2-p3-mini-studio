#ifndef UI_H__
#define UI_H__

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <list>

#include "BoutonActionBase.h"
#include "Entity.h"

class GameManager;

struct Bouton
{
	std::shared_ptr<Entity> mEntity;
	BoutonActionBase* mAction;
	sf::Vector2f pos;
	~Bouton() { mEntity.reset(); delete mAction; }
};

class UI
{
protected:	// data
	GameManager* mpGameManager;
	std::list<Bouton*> m_boutons;
	std::list<std::shared_ptr<Entity>> m_entities;
	bool actif;

protected:
	UI();
	~UI();

public:		// virtual
	virtual void OnInit() = 0;
	virtual void FixedUpdate() = 0;
	virtual void Render();
	virtual void SetActif(bool it);

protected:
	virtual void OnWindowEvent() = 0;

public:		// public finished fonction
	bool isActif();
	int GetWindowWidth() const;
	int GetWindowHeight() const;
	float GetDeltaTime() const;

	Bouton* CreateBouton(std::shared_ptr<Entity>,BoutonActionBase*);
public:		// inl (template)
	template<typename T>
	std::shared_ptr<Entity> CreateSharedEntity(float radius, const sf::Color& color);
	template<typename T>
	std::shared_ptr<Entity> CreateSharedEntity(sf::Vector2f size, const sf::Color& color);

private:	// friend
	friend class GameManager;
};

#include "UI.inl"
#endif // !UI_H__