#pragma once

#include <GameObjects/GameObject.h>
#include <Engine/Core/Constants.h>
#include <memory>

class Ball : public DynamicGameObject
{
public:
	Ball(const Vector2f& startingPos);
	~Ball() = default;

	void Update(float deltaTime) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;

	void OnCollisionEnter(IGameObject* obj) override;
	void OnCollisionStay(IGameObject* obj) override;
	void OnCollisionExit(IGameObject* obj) override;

	void ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos) override;
	void ReflectWall(const Vector2f& normal);

	void Reset() override;

	void SetPaddleSpeed(float speed) { m_paddleSpeed = speed; }
	void ResetRallieCount();
	void SetLastRallieCount();

	int GetRallieCount() const { return m_currentRallieCount; }
	int GetHighestRallieCount() const { return m_highestRallieCount; }

	void BeginRally() { m_rallieBegun = true; }
	bool RallyHasBegun() const { return m_rallieBegun; }

	bool GetRalliedEnable() const { return m_rallieEnabled; }
	void SetRalliedEnable(bool enabled);

private:

	void UpdateRallieCount(Vector2f& velocity);
	void ClampSpeed();

	bool m_collidedWithPaddle = false;
	bool m_rallieBegun = false;
	bool m_rallieEnabled = true;
	float m_edgeBuffer = 0;
	int m_rallieCount = 0;
	int m_currentRallieCount = 0;
	int m_currentRallieLimit = 0;
	int m_lastRallieCount = 0;
	int m_highestRallieCount = 0;
	float m_paddleSpeed = GameConstants::ObjectSpeed;
};