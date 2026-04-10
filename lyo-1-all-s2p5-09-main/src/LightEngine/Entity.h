#pragma once

#include "Animator.h"
#include "LightSource.h"
#include "Texture.h"

#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>


namespace sf
{
	class Shape;
	class Color;
}

struct Bouton;
class UI;

class Scene;
class Entity
{

protected:

	sf::Sprite mSprite;
	std::variant<sf::CircleShape, sf::RectangleShape> mShape;

	std::optional<LightSource> mLight;
	float mSpeed = 0.f;
	int mTag = -1;

	bool mToDestroy = false;
	bool mActive = true;
	bool mRigidBody = false;
	bool mIsStatic = false;
	bool mIsAffectByGravity = true;
	bool mIsAffectByFriction = true;
	bool mIsPlatform = false;

	Animator m_animator;

	sf::Vector2f mVelocity = { 0, 0 };
	sf::Vector2f m_spriteOffset = { 0.f, 0.f };
	sf::Vector2f m_spriteOffsetFlipped = { 0.f, 0.f };

	float mMaxFallSpeed = 400.f;
	float mMaxRunSpeed = 200.f;

public:

	Entity() = default;
	virtual ~Entity() = default;

	void setActive(bool _state) { mActive = _state; }
	void setLightSource(LightSource _newLightSource);
	void SetMaxFallSpeed(float _speed) { mMaxFallSpeed = _speed; }
	void SetMaxRunSpeed(float _speed) { mMaxRunSpeed = _speed; }
	void SetPlatform(bool _state) { mIsPlatform = _state; }
	void SetPosition(float x, float y, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetPosition(sf::Vector2f _pos, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetRigidBody(bool isRigidBody) { mRigidBody = isRigidBody; }
	void SetStatic(bool _isStatic) { mIsStatic = _isStatic; }
	void setSpeed(float _speed) { mSpeed = _speed; }
	void SetSpriteOffset(sf::Vector2f _offset) { m_spriteOffset = _offset; }
	void SetSpriteOffsetFlipped(sf::Vector2f _offset) { m_spriteOffsetFlipped = _offset; }
	void SetTag(int tag) { mTag = tag; }
	void SetVelocityX(float _velX) { mVelocity.x = _velX; }
	void SetVelocityY(float _velY) { mVelocity.y = _velY; }

	virtual void SetTexture(std::shared_ptr<Texture> _texture);


	bool GetIsPlatform() const { return mIsPlatform; }

	float GetDeltaTime() const;
	float GetMaxFallSpeed() const { return mMaxFallSpeed; }
	float GetMaxRunSpeed() const { return mMaxRunSpeed; }
	float GetRadius() const;
	float GetSpeed() const { return mSpeed; }

	const sf::Shape& GetShape() const;

	sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Sprite& GetSprite() { return mSprite; }
	sf::Vector2f GetSize() const;
	sf::Vector2f GetVelocity() const { return mVelocity; }

	std::optional<LightSource> getLightSource() const { return mLight; }

	Animator& GetAnimator() { return m_animator; }

	Scene* GetScene() const;


	bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;
	bool isRect();
	bool IsRigidBody() const { return mRigidBody; }
	bool IsStatic() const { return mIsStatic; }
	bool IsTag(int tag) const { return mTag == tag; }

	void SetAffectedByGravity(bool _state) { mIsAffectByGravity = _state; }
	void SetAffectedByFriction(bool _state) { mIsAffectByFriction = _state; }

	const bool isActive() const { return mActive; }


	bool HasTexture() { return mSprite.getTexture() != nullptr; }

	bool ToDestroy() const { return mToDestroy; }

	void AddVelocity(sf::Vector2f _newVel) { mVelocity += _newVel; }
	void Destroy();
	void switchLightSource(bool _state) { if (mLight.has_value()) mLight.value().isActive = _state; }
	void killLightSource();


	template<typename T>
	T* GetScene() const;


	template<typename T>
	T* CreateEntity(float radius, const sf::Color& color);

	template<typename T>
	T* CreateEntity(sf::Vector2f size, const sf::Color& color);


protected:



	virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnDestroy() {};
	virtual void OnFixedUpdate() {};
	virtual void OnInitialize() {};

private:

	void ApplyGravity();
	void FixedUpdate();
	void Initialize(float radius, const sf::Color& color);
	void Initialize(sf::Vector2f size, const sf::Color& color);
	void Repulse(Entity* other);

	friend class GameManager;
	friend class TeleportPlayerAction;
	friend class QuadTree;
	friend Bouton;
	friend UI;
	friend Scene;
};

#include "Entity.inl"
