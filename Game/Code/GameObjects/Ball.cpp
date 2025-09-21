#include "Ball.h"

#include "Player.h"
#include "../Utilities/GameMode.h"
#include "../Utilities/DifficultyMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Collisions/BoundingCircle.h>
#include <algorithm>
#include <numbers>
#include <random>

// Global or static random engine to avoid re-seeding every call
std::random_device nrd;
std::mt19937 ngen(nrd());
std::bernoulli_distribution ncoinFlip(0.5f);  // 50/50 chance

float NRandomOneOrMinusOne()
{
	return ncoinFlip(ngen) ? 1.0f : -1.0f;
}

Ball::Ball(const Vector2f& startingPos)
	: DynamicGameObject(std::make_shared<SFSprite>("Ball"), std::make_shared<BoundingCircle<SFCircle>>(11.04f, startingPos))
{
	SetInitialPosition(startingPos);
	SetPosition(GetInitialPosition());
	SetXVelocity(NRandomOneOrMinusOne() * GameConstants::ObjectSpeed);
	SetInitialDirection(GetXVelocity() > 0);
	SetDirection(GetInitialDirection());

	auto circle = dynamic_cast<BoundingCircle<SFCircle>*>(m_volume.get());
	circle->Update(GetPosition());

	float radius = circle->GetRadius();

	m_edgeBuffer = radius + DifficultyMode::wallThickness;

	m_currentRallieLimit = DifficultyMode::rallieLimit;
}

void Ball::Update(float deltaTime)
{
	SetPrevPosition(GetPosition());

	if (GetVelocity() != Vector2f(0, 0))
	{
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
		Move(GetVelocity() * GameConstants::FPS * deltaTime);
	}
}

void Ball::Move(float x, float y)
{
	auto spr = dynamic_cast<SFSprite*>(m_drawable.get());
	if (spr)
		spr->Move(x, y);

	auto circle = dynamic_cast<BoundingCircle<SFCircle>*>(m_volume.get());
	if (circle)
		circle->Update(GetPosition());
}

void Ball::Move(const Vector2f& mov)
{
	Move(mov.x, mov.y);
}

bool Ball::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	bool col = false;
	if (DynamicGameObject::Intersects(obj, tFirst, tLast))
	{
		if (Player* ply = dynamic_cast<Player*>(obj))
		{
			if (m_collidedWithPaddle)
			{
				OnCollisionStay(obj);
				obj->OnCollisionStay(this);
			}
			else
			{
				OnCollisionEnter(obj);
				obj->OnCollisionEnter(this);
			}
		}

		col = true;
	}
	else
	{
		col = false;

		if (m_collidedWithPaddle)
		{
			OnCollisionExit(obj);
			obj->OnCollisionExit(this);
		}
	}

	return col;
}

void Ball::OnCollisionEnter(IGameObject* obj)
{
	m_collidedWithPaddle = true;
	m_rallieEnabled = true;
}

void Ball::OnCollisionStay(IGameObject* obj)
{
	m_rallieEnabled = false;
}

void Ball::OnCollisionExit(IGameObject* obj)
{
	m_collidedWithPaddle = false;
}

void Ball::ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos)
{
	Vector2f velocity = GetVelocity();
	Vector2f position = GetPosition();

	// Step 1: Move to the point of collision using velocity and time
	position += velocity * time;

	// Step 2: Apply full separation vector to push ball out of collision
	position += sepVec;

	SetPosition(position);
	m_volume->Update(position);

	// Step 3: Normalize the collision normal (if applicable)
	Vector2f normal = (sepVec != Vector2f(0.f, 0.f)) ? sepVec.Normalize() : Vector2f(0.f, 0.f);

	// Step 4: Calculate bounce direction based on relative hit position
	relHitPos = std::clamp(relHitPos, -1.f, 1.f);
	Vector2f newVelocity;

	if (relHitPos == 0.0f)
	{
		// Pure horizontal reflection (e.g. center hit)
		newVelocity.x = -velocity.x;
		newVelocity.y = velocity.y;
	}
	else
	{
		// Calculate angle based on hit position (adds curvature to bounce)
		float bounceAngle = relHitPos * (std::numbers::pi_v<float> / 4); // ±45 degrees max
		float speed = velocity.Length();

		float direction = GetDirection() ? -1.f : 1.f;

		newVelocity.x = direction * speed * std::cos(bounceAngle);
		newVelocity.y = speed * std::sin(bounceAngle);
	}

	// Step 5: Optional rally mechanic (increase speed after X bounces)
	if (m_rallieBegun && m_rallieEnabled)
		UpdateRallieCount(velocity);

	// Step 6: Apply velocity and clamp
	SetVelocity(newVelocity);
	ClampSpeed();
	SetDirection(GetXVelocity() > 0);
}

void Ball::ReflectWall(const Vector2f& normal)
{
	auto velocity = GetVelocity();

	// Compute dot product of velocity and normal
	float dotProduct = velocity.Dot(normal);

	// Reflection formula: V' = V - 2 * (V . N) * N
	Vector2f reflected;
	reflected.x = velocity.x - 2 * dotProduct * normal.x;
	reflected.y = velocity.y - 2 * dotProduct * normal.y;

	SetVelocity(reflected);
	ClampSpeed();
}

void Ball::Reset()
{
	SetActive(false);
	SetDirection(!GetDirection());
	SetPosition(GetInitialPosition());
	m_volume->Update(Vector2f(GetPosition().x, GetPosition().y + 3.5f));
	SetVelocity(Vector2f(DifficultyMode::paddleSpeed * -1, 0));
	ResetRallieCount();
}

void Ball::ResetRallieCount()
{
	m_rallieBegun = m_rallieEnabled = (GameMode::s_type == vsWall) ? false : true;
	m_rallieCount = 0;
	m_currentRallieCount = 0;
	m_currentRallieLimit = DifficultyMode::rallieLimit;
}

void Ball::SetLastRallieCount()
{
	m_lastRallieCount = m_currentRallieCount;
	m_highestRallieCount = std::max(m_lastRallieCount, m_highestRallieCount);
}

void Ball::SetRalliedEnable(bool enabled)
{
	if (m_rallieBegun)
		m_rallieEnabled = enabled;
}

void Ball::UpdateRallieCount(Vector2f& velocity)
{
	m_currentRallieCount++;
	if (m_rallieCount++ == m_currentRallieLimit)
	{
		velocity.x = (velocity.x *= DifficultyMode::speedFactor);
		m_rallieCount = 0;
		m_currentRallieLimit *= 2;
	}
}

void Ball::ClampSpeed()
{
	Vector2f velocity = GetVelocity();
	float speed = velocity.Length();  // Get speed magnitude

	float minSpeed = -GameConstants::ObjectSpeed - 2.f;
	float maxSpeed = GameConstants::ObjectSpeed + 2.f;

	if (speed < minSpeed)
		velocity.x = velocity.Normalize().x * minSpeed;
	else if (speed > maxSpeed)
		velocity.x = velocity.Normalize().x * maxSpeed;

	SetVelocity(velocity);
}