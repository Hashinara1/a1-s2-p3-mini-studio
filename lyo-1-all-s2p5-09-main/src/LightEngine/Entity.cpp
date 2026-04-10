#include "Debug.h"
#include "Entity.h"
#include "GameManager.h"
#include "LightManager.h"
#include "Utils.h"

#include <iostream>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>



static void RepulseRectCircle(Entity* rectEntity, Entity* circleEntity,
	const sf::RectangleShape& rect, const sf::CircleShape& circle,
	float rectForce, float circleForce)
{
	//we find the nearest point
	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();

	sf::Vector2f circleCenter = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
	float radius = circle.getRadius();

	float closestX = std::max(rectPos.x, std::min(circleCenter.x, rectPos.x + rectSize.x));
	float closestY = std::max(rectPos.y, std::min(circleCenter.y, rectPos.y + rectSize.y));

	//we look at how much we are in the other
	sf::Vector2f diff = circleCenter - sf::Vector2f(closestX, closestY);
	float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

	sf::Vector2f normal;
	float penetration;

	if (dist == 0.f)
	{
		// if we are right in the center
		float dLeft = circleCenter.x - rectPos.x;
		float dRight = rectPos.x + rectSize.x - circleCenter.x;
		float dTop = circleCenter.y - rectPos.y;
		float dBottom = rectPos.y + rectSize.y - circleCenter.y;
		float minD = std::min(std::min(dLeft, dRight), std::min(dTop, dBottom));

		if (minD == dLeft) normal = { -1.f,  0.f };
		else if (minD == dRight) normal = { 1.f,  0.f };
		else if (minD == dTop) normal = { 0.f, -1.f };
		else normal = { 0.f,  1.f };
		penetration = radius + minD;
	}
	else
	{
		normal = diff / dist;
		penetration = radius - dist;
	}

	//we push outwards
	sf::Vector2f translation = normal * (penetration * 0.5f);

	sf::Vector2f centerRect = rectEntity->GetPosition(0.5f, 0.5f);
	sf::Vector2f centerCircle = circleEntity->GetPosition(0.5f, 0.5f);

	sf::Vector2f position1 = centerRect - translation * rectForce;
	sf::Vector2f position2 = centerCircle + translation * circleForce;



	rectEntity->SetPosition(position1.x, position1.y, 0.5f, 0.5f);
	circleEntity->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
}

static bool RectCircleCollision(const sf::RectangleShape& rect, const sf::CircleShape& circle)
{
	//find the point closest to the circle

	sf::Vector2f center = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
	sf::Vector2f rectPos = rect.getPosition();
	sf::Vector2f rectSize = rect.getSize();

	float closestX = std::max(rectPos.x, std::min(center.x, rectPos.x + rectSize.x));
	float closestY = std::max(rectPos.y, std::min(center.y, rectPos.y + rectSize.y));

	float dx = center.x - closestX;
	float dy = center.y - closestY;
	return (dx * dx + dy * dy) < (circle.getRadius() * circle.getRadius());
}




void Entity::Initialize(float radius, const sf::Color& color)
{
	std::visit([radius, color](auto& shape) {
		if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, sf::CircleShape>) {
			shape.setOrigin(0.f, 0.f);
			shape.setRadius(radius);
			shape.setFillColor(color);
		}
		}, mShape);


	OnInitialize();
}

void Entity::Initialize(sf::Vector2f size, const sf::Color& color)
{
	mShape = sf::RectangleShape{};
	std::visit([size, color](auto& shape) {
		if constexpr (std::is_same_v<std::decay_t<decltype(shape)>, sf::RectangleShape>) {
			shape.setOrigin(0.f, 0.f);
			shape.setSize(size);
			shape.setFillColor(color);
		}
		}, mShape);


	OnInitialize();
}

void Entity::Repulse(Entity* other)
{

	// A dynamic entity is one that moves (non-static).
	// We reset its velocity, regardless of the creation order in mEntities

	Entity* dyn = mIsStatic ? other : this;


	std::visit([this, other, dyn](auto& shapeA, auto& shapeB) {
		using T = std::decay_t<decltype(shapeA)>;
		using G = std::decay_t<decltype(shapeB)>;

		float shapeAForce = 0.5f;
		float shapeBForce = 0.5f;

		if (this->mIsStatic) {
			shapeAForce = 0.f;
			shapeBForce *= 2;
		}
		if (other->mIsStatic) {
			shapeBForce = 0.f;
			shapeAForce *= 2;
		}

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::CircleShape>) {

			sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);
			float length = std::sqrt(distance.x * distance.x + distance.y * distance.y);
			if (length == 0.f) return;

			float overlap = (length - (shapeA.getRadius() + shapeB.getRadius())) * 0.5f;
			if (overlap >= 0.f) return;

			sf::Vector2f normal = distance / length;
			sf::Vector2f translation = overlap * normal;

			

			bool isFloor = (dyn == this) ? (normal.y < 0) : (normal.y > 0);
			if (dyn->mVelocity.y > 0 && isFloor) dyn->mVelocity.y = 0;

			sf::Vector2f position1 = GetPosition(0.5f, 0.5f) - translation * shapeAForce;
			sf::Vector2f position2 = other->GetPosition(0.5f, 0.5f) + translation * shapeBForce;
			SetPosition(position1.x, position1.y, 0.5f, 0.5f);
			other->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
		}

		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::RectangleShape>)
		{
			sf::Vector2f pos1 = shapeA.getPosition(), size1 = shapeA.getSize();
			sf::Vector2f pos2 = shapeB.getPosition(), size2 = shapeB.getSize();
			sf::Vector2f center1 = GetPosition(0.5f, 0.5f);
			sf::Vector2f center2 = other->GetPosition(0.5f, 0.5f);

			float overlapX = std::min(pos1.x + size1.x, pos2.x + size2.x) - std::max(pos1.x, pos2.x);
			float overlapY = std::min(pos1.y + size1.y, pos2.y + size2.y) - std::max(pos1.y, pos2.y);
			if (overlapX <= 0 || overlapY <= 0) return;

			sf::Vector2f translation;
			bool isFloor = false;
			if (overlapX < overlapY) {
				float sign = (center1.x < center2.x) ? 1.f : -1.f;
				translation = { overlapX * 0.5f * sign, 0.f };
				// horizontal collision, not a ground
			} else {
				float sign = (center1.y < center2.y) ? 1.f : -1.f;
				translation = { 0.f, overlapY * 0.5f * sign };
				// vertical collision with the ground
				Entity* staticE = (dyn == this) ? other : this;
				isFloor = dyn->GetPosition(0.5f, 0.5f).y < staticE->GetPosition(0.5f, 0.5f).y;
			}
			if (dyn->mVelocity.y > 0 && isFloor) dyn->mVelocity.y = 0;

			SetPosition(center1.x - translation.x * shapeAForce, center1.y - translation.y * shapeAForce, 0.5f, 0.5f);
			other->SetPosition(center2.x + translation.x * shapeBForce, center2.y + translation.y * shapeBForce, 0.5f, 0.5f);
		}

		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::CircleShape>) {
			sf::Vector2f circleCenter = other->GetPosition(0.5f, 0.5f);
			sf::Vector2f rectPos = shapeA.getPosition(), rectSize = shapeA.getSize();
			float closestY = std::max(rectPos.y, std::min(circleCenter.y, rectPos.y + rectSize.y));
			// ground = circle above the point of contact on the rectangle
			bool isFloor = circleCenter.y <= closestY;
			if (dyn->mVelocity.y > 0 && isFloor) dyn->mVelocity.y = 0;
			RepulseRectCircle(this, other, shapeA, shapeB, shapeAForce, shapeBForce);
		}

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::RectangleShape>) {
			sf::Vector2f circleCenter = GetPosition(0.5f, 0.5f);
			sf::Vector2f rectPos = shapeB.getPosition(), rectSize = shapeB.getSize();
			float closestY = std::max(rectPos.y, std::min(circleCenter.y, rectPos.y + rectSize.y));
			// ground = circle above the point of contact on the rectangle
			bool isFloor = circleCenter.y <= closestY;
			if (dyn->mVelocity.y > 0 && isFloor) dyn->mVelocity.y = 0;
			RepulseRectCircle(other, this, shapeB, shapeA, shapeBForce, shapeAForce);
		}

		}, mShape, other->mShape);

}

bool Entity::IsColliding(Entity* other) const
{

	return std::visit([other, this](const auto& shapeA, const auto& shapeB) -> bool {
		using T = std::decay_t<decltype(shapeA)>;
		using G = std::decay_t<decltype(shapeB)>;

		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::CircleShape>) {

			//circle circle
			sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

			float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

			float radius1 = GetRadius();
			float radius2 = other->GetRadius();

			float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

			return sqrLength < sqrRadius;
		}

		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::RectangleShape>) {

			//rect rect
			sf::Vector2f size1 = shapeA.getSize();
			sf::Vector2f pos1 = shapeA.getPosition();


			sf::Vector2f size2 = shapeB.getSize();
			sf::Vector2f pos2 = shapeB.getPosition();


			return pos1.x < pos2.x + size2.x &&
				pos2.x < pos1.x + size1.x &&
				pos1.y < pos2.y + size2.y &&
				pos2.y < pos1.y + size1.y;
		}
		if constexpr (std::is_same_v<T, sf::RectangleShape> && std::is_same_v<G, sf::CircleShape>)
			return RectCircleCollision(shapeA, shapeB);
		if constexpr (std::is_same_v<T, sf::CircleShape> && std::is_same_v<G, sf::RectangleShape>)
			return RectCircleCollision(shapeB, shapeA);


		}, mShape, other->mShape);
}

bool Entity::IsInside(float x, float y) const
{
	return std::visit([x, y, this](auto& shape) -> bool {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			sf::Vector2f position = GetPosition(0.5f, 0.5f);

			float dx = x - position.x;
			float dy = y - position.y;

			float radius = GetRadius();

			return (dx * dx + dy * dy) < (radius * radius);
		}
		else {
			sf::Vector2f size = shape.getSize();
			sf::Vector2f position = shape.getPosition();

			return x > position.x && y > position.y && x < position.x + size.x && y < position.y + size.y;
		}
		}, mShape);
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
	sf::Vector2f size = std::visit([](auto& shape) -> sf::Vector2f {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			float size = shape.getRadius() * 2;
			return { size, size };
		}
		else
			return shape.getSize();
		}, mShape);

	x -= size.x * ratioX;
	y -= size.y * ratioY;

	std::visit([x, y](auto& shape) {shape.setPosition(x, y); }, mShape);
	bool flipped1 = mSprite.getScale().x < 0;
	float flipOffsetX1 = flipped1 ? mSprite.getLocalBounds().width * std::abs(mSprite.getScale().x) : 0.f;
	sf::Vector2f offset1 = flipped1 ? m_spriteOffsetFlipped : m_spriteOffset;
	mSprite.setPosition(x + offset1.x + flipOffsetX1, y + offset1.y);

}

void Entity::SetPosition(sf::Vector2f _pos, float ratioX, float ratioY)
{
	sf::Vector2f size = std::visit([](auto& shape) -> sf::Vector2f {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			float size = shape.getRadius() * 2;
			return { size, size };
		}
		else
			return shape.getSize();
		}, mShape);

	_pos.x -= size.x * ratioX;
	_pos.y -= size.y * ratioY;

	std::visit([_pos](auto& shape) {shape.setPosition(_pos.x, _pos.y); }, mShape);
	bool flipped2 = mSprite.getScale().x < 0;
	float flipOffsetX2 = flipped2 ? mSprite.getLocalBounds().width * std::abs(mSprite.getScale().x) : 0.f;
	sf::Vector2f offset2 = flipped2 ? m_spriteOffsetFlipped : m_spriteOffset;
	mSprite.setPosition(_pos.x + offset2.x + flipOffsetX2, _pos.y + offset2.y);
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	return std::visit([ratioX, ratioY, this](auto& shape) -> sf::Vector2f {

		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			float size = shape.getRadius() * 2;
			sf::Vector2f position = shape.getPosition();

			position.x += size * ratioX;
			position.y += size * ratioY;

			return position;
		}
		else {
			sf::Vector2f size = shape.getSize();
			sf::Vector2f position = shape.getPosition();

			position.x += size.x * ratioX;
			position.y += size.y * ratioY;

			return position;

		}
		}, mShape);
}

const sf::Shape& Entity::GetShape() const 
{
	return std::visit([]( auto& shape) -> const sf::Shape& { return shape; }, mShape);
}


bool Entity::isRect()
{
	
	return	std::visit([](auto& shape) -> bool {
		using T = std::decay_t<decltype(shape)>;

		if constexpr (std::is_same_v<T, sf::CircleShape>) {
			return false; 
		}
		else {
			return true;
		}
		}, mShape);
	
}

void Entity::SetTexture(std::shared_ptr<Texture> _texture)
{
	mSprite.setTexture(*_texture->getTexture());

	sf::Vector2f size = std::visit([](const auto& shape) -> sf::Vector2f {
		using T = std::decay_t<decltype(shape)>;
		if constexpr (std::is_same_v<T, sf::CircleShape>)
		{
			float diameter = shape.getRadius() * 2;
			return { diameter, diameter };
		}
		else
			return shape.getSize();
		}, mShape);

	//float scaleX = size.x / _texture->getWidth();
	//float scaleY = size.y / _texture->getHeight();
	//mSprite.setScale(scaleX, scaleY);

}


void Entity::setLightSource(LightSource _newLightSource)
{
	mLight = _newLightSource; 
	LightManager::getInstance().addLight(this);
}

void Entity::killLightSource()
{
	mLight.reset();

	LightManager::getInstance().removeLight(this);
}


float Entity::GetRadius() const
{
	return std::visit([](auto& shape) -> float
		{
			using T = std::decay_t<decltype(shape)>;
			if constexpr (std::is_same_v < T, sf::CircleShape>)
				return shape.getRadius();
			else
				return 0.f;
		}, mShape);
}
sf::Vector2f Entity::GetSize() const
{
	return std::visit([](auto& shape) -> sf::Vector2f
		{
			using T = std::decay_t<decltype(shape)>;
			if constexpr (std::is_same_v < T, sf::RectangleShape>)
				return shape.getSize();
			else
				return sf::Vector2f{0,0};
		}, mShape);
}

void Entity::ApplyGravity()
{
	if (mIsStatic || !mRigidBody || !mIsAffectByGravity)
		return;

	mVelocity += GameManager::Get()->GetScene()->GetGravity();
}
void Entity::FixedUpdate()
{
	if (mIsStatic)
		return;

	OnFixedUpdate();

	//we cap the velocity at mMa Velocity max
	if (std::fabs(mVelocity.x) > mMaxRunSpeed) {
		mVelocity.x = mMaxRunSpeed * (mVelocity.x > 0 ? 1 : -1);
	}
	if (std::fabs(mVelocity.y) > mMaxFallSpeed) {
		mVelocity.y = mMaxFallSpeed * (mVelocity.y > 0 ? 1 : -1);
	}

	if (mIsAffectByFriction) {
		//The entity is slowed down (friction only on X, gravity manages Y)
		mVelocity.x -= mVelocity.x / 20;
		if (std::fabs(mVelocity.x) < 50.f)
			mVelocity.x = 0;
	}

	sf::Vector2f translation = mVelocity * GameManager::Get()->GetDeltaTime();

	std::visit([translation](auto& shape) {
		shape.move(translation);
		}, mShape);

	bool flipped = mSprite.getScale().x < 0;
	float flipOffsetX = flipped ? mSprite.getLocalBounds().width * std::abs(mSprite.getScale().x) : 0.f;
	sf::Vector2f offset = flipped ? m_spriteOffsetFlipped : m_spriteOffset;
	mSprite.setPosition(GetPosition(0.f, 0.f) + sf::Vector2f(offset.x + flipOffsetX, offset.y));



}
Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}
