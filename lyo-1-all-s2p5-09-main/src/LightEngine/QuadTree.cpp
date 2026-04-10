#include "QuadTree.h"

#include "GameManager.h"
#include "Debug.h"

#include <iostream>

//#define DEBUG

constexpr int kEntityListMaxSize = 8;
constexpr int kQuadTreeMaxSize = 4;

/*		QuadTree Objectif:
* Reset and Reconstruct every FixedUpdate					check
*
* Take List and for each Entity add to QuadTree
*		if (QuadTree Full) Divide							check
*		if (Divided) check if Entity go in Divided Quad		check
*
* Collision take in account Parent Quad						check
*/

QuadTree::QuadTree(Rect boundary) :
	m_rect(boundary),
	divided(false)
{
	Clear();
}
QuadTree::~QuadTree()
{
	//	Delete all Quad cr�ated
	if (divided)
		for (QuadTree* it : m_quad)
			delete it;
}
void QuadTree::Clear()
{
	//	Clear all data but the pos and size
	if (divided)
	{
		for (QuadTree* it : m_quad)
		{
			delete it;
		}
		m_quad.clear();
	}
	else
		m_entities.clear();

	divided = false;
}
void QuadTree::Divide()
{
	sf::Vector2f size = m_rect.size;
	size.x /= 2;
	size.y /= 2;
	//	Create Child Quad in the order of NW->NE->SW->SE
	for (int i = 0; i < kQuadTreeMaxSize; i++)
	{
		Rect tmp = { m_rect.pos, size };
		if (i & 1)
			tmp.pos.x += size.x;
		if (i & (1 << 1))
			tmp.pos.y += size.y;
		m_quad.push_back(new QuadTree(tmp));
	}
	divided = true;

	//	add to quad if fully in it then remove the pointeur if yes;
	for (auto it = m_entities.begin(); it != m_entities.end(); )
	{
		bool removed = false;
		for (auto it2 : m_quad)
			if (it2->Add(*it))
			{
				it = m_entities.erase(it); // erase and take the next iteration
				removed = true;
				break;
			}
		if (!removed)
			++it;	// go to next iteration
	}
}
bool QuadTree::Add(Entity* add)
{
	if (!add)
		return false;
	//	if not fully in the quad return false
	if (add->GetPosition(0, 0).x < m_rect.pos.x || add->GetPosition(1, 1).x > m_rect.pos.x + m_rect.size.x
		|| add->GetPosition(0, 0).y < m_rect.pos.y || add->GetPosition(1, 1).y > m_rect.pos.y + m_rect.size.y)
		return false;

	//	Add to child if divided, else add to it if not max size, else divide and create child;
	if (divided)
		AddToChild(add);
	else
	{
		m_entities.push_back(add);
		if (m_entities.size() >= kEntityListMaxSize)
			Divide();
	}

	return true;
}
void QuadTree::AddToChild(Entity* add)
{
	//	Add if fully in child Quad, else add to this quad
	bool added = false;
	for (QuadTree* it : m_quad)
		if (it->Add(add))
		{
			added = true;
			break;
		}
	if (!added)
		this->m_entities.push_back(add);
}

//	only used for the Origine Quad
void QuadTree::init(std::list<Entity*>& entities)
{
	//	Delete all Quad then recreate them
	Clear();
	for (Entity* it : entities)
		if (!Add(it))
		{
#ifdef DEBUG
			std::cout << "data lost \n";
#endif // DEBUG
		}
}
void QuadTree::Collision(std::list<Entity*>* target)
{
	if (target == nullptr)
		target = &m_entities;
	for (Entity* it : m_entities)
		for (Entity* it2 : *target)
		{
			//	check if the same or is not colliding
			if (it == it2 || !it->IsColliding(it2))
				continue;

			if (it->IsRigidBody() && it2->IsRigidBody())
				if (it->mIsPlatform == false && it2->mIsPlatform == false)
					it->Repulse(it2);
				else {
					if (it->GetIsPlatform()) {
						float prevBottom = it2->GetPosition(0, 1).y - it2->GetVelocity().y;
						if (prevBottom <= it->GetPosition(0, 0).y)
							it->Repulse(it2);
					}
					else if (it2->GetIsPlatform()) {
						float prevBottom = it->GetPosition(0, 1).y - it->GetVelocity().y;
						if (prevBottom <= it2->GetPosition(0, 0).y)
							it2->Repulse(it);
					}
				}
			it->OnCollision(it2);
			it2->OnCollision(it);
		}
}

void QuadTree::UpdateChildCollision(std::list<Entity*>& tmp)
{
	//	Collision for each entity of this list then do collision of the parental list (tmp)
	Collision();
	Collision(&tmp);

#ifdef DEBUG
	Debug::DrawRectangle(m_rect.pos.x, m_rect.pos.y, m_rect.size.x, m_rect.size.y, sf::Color::Green);
#endif // DEBUG

	//	if divided merge the list and pass to the enfant the parental entity list
	if (divided)
	{
		for (Entity* it : tmp)
			m_entities.push_back(it);
		for (QuadTree* it : m_quad)
			it->UpdateChildCollision(m_entities);
	}
}
void QuadTree::UpdateCollision()
{
	Collision();

#ifdef DEBUG
	Debug::DrawRectangle(m_rect.pos.x, m_rect.pos.y, m_rect.size.x, m_rect.size.y, sf::Color::Green);
#endif // DEBUG

	//	UpdateCollision of child and pass the Entities
	if (divided)
		for (QuadTree* it : m_quad)
			it->UpdateChildCollision(m_entities);
}