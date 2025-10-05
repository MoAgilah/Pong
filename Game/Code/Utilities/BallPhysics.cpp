#include "BallPhysics.h"

#include "DifficultyMode.h"
#include "../GameObjects/Ball.h"
#include <Utilities/Utils.h>

Ball* BallPhysics::m_currBall = nullptr;

void BallPhysics::SetCurrentBall(Ball* ball)
{
	m_currBall = ball;
	ENSURE_VALID(m_currBall);

}

Vector2f BallPhysics::GetVelocity()
{
	ENSURE_VALID_RET(m_currBall, Vector2f());
	return m_currBall->GetVelocity();
}

Vector2f BallPhysics::GetPosition()
{
	ENSURE_VALID_RET(m_currBall, Vector2f());
	return m_currBall->GetPosition();
}

void BallPhysics::ApplyTopSpin(float spinFactor)
{
	ENSURE_VALID(m_currBall);
	if (m_currBall->GetDirection())
		m_currBall->DecrementYVelocity(spinFactor);
	else
		m_currBall->IncrementYVelocity(spinFactor);
}

void BallPhysics::ApplyBackSpin(float spinFactor)
{
	ENSURE_VALID(m_currBall);
	if (m_currBall->GetDirection())
		m_currBall->IncrementYVelocity(spinFactor);
	else
		m_currBall->DecrementYVelocity(spinFactor);
}

void BallPhysics::ApplyLeftSpin(float spinFactor)
{
	ENSURE_VALID(m_currBall);
	if (m_currBall->GetDirection())
		m_currBall->IncrementXVelocity(spinFactor);
	else
		m_currBall->DecrementXVelocity(spinFactor);
}

void BallPhysics::ApplyRightSpin(float spinFactor)
{
	ENSURE_VALID(m_currBall);
	if (m_currBall->GetDirection())
		m_currBall->DecrementXVelocity(spinFactor);
	else
		m_currBall->IncrementXVelocity(spinFactor);
}

void BallPhysics::ApplyVelocityIncrease(float speedFactor)
{
	if (m_currBall)
		m_currBall->SetVelocity(m_currBall->GetVelocity() * speedFactor);
}

void BallPhysics::ApplyVelocityDecrease(float speedFactor)
{
	if (m_currBall)
		m_currBall->SetVelocity(m_currBall->GetVelocity() / speedFactor);
}