#ifndef QUAD_TREE_H__
#define QUAD_TREE_H__

#include "Entity.h"

#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>

class Rect
{
public:

    Rect(sf::Vector2f setPos, sf::Vector2f setSize) : pos(setPos), size(setSize) {};

    sf::Vector2f pos;
    sf::Vector2f size;
};
class QuadTree
{
    Rect m_rect;
    std::list<Entity*> m_entities;

    std::list<QuadTree*> m_quad;

    bool divided;

private:

    ~QuadTree();

    void Clear();
    void Divide();

    bool Add(Entity*);

    void AddToChild(Entity*);

    void Collision(std::list<Entity*>* = nullptr);
    void UpdateChildCollision(std::list<Entity*>& tmp);

public:

    QuadTree(Rect);

    void init(std::list<Entity*>&);
    void UpdateCollision();

private:

    friend class GameManager;
};

#endif // !QUAD_TREE_H__