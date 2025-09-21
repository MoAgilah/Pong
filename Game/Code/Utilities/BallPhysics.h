#pragma once

#include <Utilities/Vector.h>

class Ball;

class BallPhysics
{
public:
	BallPhysics() = default;
	~BallPhysics() = default;

	void SetCurrentBall(Ball* ball);

	static Vector2f GetVelocity();
	static Vector2f GetPosition();

	static void ApplyTopSpin(float spinFactor);
	static void ApplyBackSpin(float spinFactor);
	static void ApplyLeftSpin(float spinFactor);
	static void ApplyRightSpin(float spinFactor);

	static void ApplyVelocityIncrease(float speedFactor);
	static void ApplyVelocityDecrease(float speedFactor);

private:

	static Ball* m_currBall;
};