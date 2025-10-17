#pragma once

#include "../Engine/AI/PongAICtrl.h"
#include <Engine/Input/KeyCode.h>
#include <Engine/Core/Constants.h>
#include <GameObjects/GameObject.h>
#include <array>
#include <functional>
#include <random>

enum Results;

enum PlayerIdentifiers
{
	Player1,
	Player2,
	MaxPlayers,
	Unset = -1
};

enum Keys
{
	MoveUpKey,
	MoveDownKey,
	FastShotKey,
	SlowShotKey,
	TopSpinKey,
	BackSpinKey,
	LeftSpinKey,
	RightSpinKey,
	MaxKeys,
};

class Player : public DynamicGameObject
{
public:
	Player(PlayerIdentifiers plyID);
	~Player() = default;

	void Update(float deltaTime) override;

	bool Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast) override;

	void OnCollisionEnter(IGameObject* obj) override;
	void OnCollisionStay(IGameObject* obj) override;
	void OnCollisionExit(IGameObject* obj) override;

	void ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos) override;

	bool HasCollidedWithBall() const { return m_collidedWithBall; }

	void Reset() override;

	PlayerIdentifiers GetPlayerID() const { return m_playerID; }

protected:

	float m_edgeBuffer = 0;
	bool m_hasAppliedShot = false;
	virtual void SetCollisionWithBall(bool hasCollided);

private:

	void ProcessInput();

	static int s_playerNum;
	int m_verticalWall = 0;
	bool m_collidedWithBall = false;
	float m_paddleSpeed = GameConstants::ObjectSpeed * 0.8f;
	std::array<KeyCode, MaxKeys> m_keys;
	std::vector<KeyCode> m_specialKeys;
	PlayerIdentifiers m_playerID;
};

class AutomatedPlayer : public Player
{
public:
	AutomatedPlayer(PlayerIdentifiers plyID);
	~AutomatedPlayer() = default;

	void Update(float deltaTime) override;

	void UpdateFatigue(int rallyCount);
	void UpdateStress(int myScore, int oppScore);

	bool Intersects(IGameObject* obj) override;

	void Reset() override;

protected:

	void SetCollisionWithBall(bool hasCollided) override;

private:

	bool ShouldReactToBall(const Vector2f& ballVel);

	Player* GetOpponent();

	bool BiasedRandomChoice(float biasFactor, bool& lastResult);

	float StandardShot();
	float AggressiveShot();
	float DefensiveShot();

	float FastShot();
	float SlowShot();
	float ChangeUpShot();

	float TopSpinShot();
	float BackSpinShot();
	float LengthSpinShot();

	float LeftSpinShot();
	float RightSpinShot();
	float WidthSpinShot();

	float FakeShot(int playstyle);
	float CornerShot();
	float WallShot();
	float EdgeShot();

	float GetShotOffset();

	float GenerateReactionDelay();

	void PredictBallTarget(const Vector2f& paddlePos, const Vector2f& ballVel);

	void UpdateMovement(const Vector2f& paddlePos, float deltaTime);

	void ApplyPendingShot();

	// AI Control System
	PongAICtrl m_aiCtrl;

	// Pointers to Game Objects
	Player* m_opponent = nullptr;

	// Random Number Generation (Seeding)
	std::random_device m_rd;
	std::mt19937 m_gen;

	// Time-Related Variables (Delays & Timers)
	float m_bufferTime = 0;
	float m_timeToPaddle = 0;
	float m_reactionTimer = 0;
	float m_reactionDelay = 0;
	float m_timeDelay = 0;

	// Ball & Paddle Prediction Variables
	float m_predictedY = 0;
	float m_targetY = 0;
	float m_fatigue = 0;
	float m_stress = 0;

	// Spin & Shot Logic
	bool m_shotReady = false;
	int m_pendingShot = ShotTypes::StandardShot;
	bool m_lastSpeedShot = (std::rand() % 2 == 0);
	bool m_lastWidthSpin = (std::rand() % 2 == 0);
	bool m_lastLengthSpin = (std::rand() % 2 == 0);
};