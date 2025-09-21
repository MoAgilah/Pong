#include "Player.h"

#include "Ball.h"
#include "../Utilities/BallPhysics.h"
#include "../Utilities/DifficultyMode.h"
#include "../Utilities/MatchCtrl.h"
#include "../Engine/States/PauseMenuState.h"
#include "../Engine/UI/ScoreBoard.h"
#include <Drawables/SFSprite.h>
#include <Drawables/SFShape.h>
#include <Engine/Collisions/BoundingCapsule.h>
#include <Engine/Core/GameManager.h>
#include <ranges>

int Player::s_playerNum = -1;

Player::Player(PlayerIdentifiers plyID)
	: DynamicGameObject(std::make_shared<SFSprite>("Paddle"), std::make_shared<BoundingCapsule<SFCapsule>>(6.44f, 99.84f, 0.f, Vector2f()))
{
	if (++s_playerNum == PlayerIdentifiers::MaxPlayers)
		throw std::out_of_range("Maximum players have already been constructed");

	m_playerID = plyID;

	m_keys = {};

	switch (m_playerID)
	{
	case Player1:
		m_keys[Keys::MoveUpKey] = KeyCode::W;
		m_keys[Keys::MoveDownKey] = KeyCode::S;
		m_keys[Keys::FastShotKey] = KeyCode::LShift;
		m_keys[Keys::SlowShotKey] = KeyCode::LControl;
		m_keys[Keys::LeftSpinKey] = KeyCode::A;
		m_keys[Keys::RightSpinKey] = KeyCode::D;
		m_keys[Keys::TopSpinKey] = KeyCode::Q;
		m_keys[Keys::BackSpinKey] = KeyCode::Z;

		SetInitialPosition(Vector2f(50.f, GameConstants::ScreenDim.y / 2.0f));

		break;
	case Player2:
		m_keys[Keys::MoveUpKey] = KeyCode::I;
		m_keys[Keys::MoveDownKey] = KeyCode::K;
		m_keys[Keys::FastShotKey] = KeyCode::RShift;
		m_keys[Keys::SlowShotKey] = KeyCode::RControl;
		m_keys[Keys::LeftSpinKey] = KeyCode::J;
		m_keys[Keys::RightSpinKey] = KeyCode::L;
		m_keys[Keys::TopSpinKey] = KeyCode::U;
		m_keys[Keys::BackSpinKey] = KeyCode::M;

		SetInitialPosition(Vector2f(GameConstants::ScreenDim.x - 50.f, GameConstants::ScreenDim.y / 2.0f));

		break;
	}

	for (auto it = m_keys.begin() + Keys::FastShotKey; it != m_keys.end(); ++it)
		m_specialKeys.push_back(*it);

	SetScale({ GetScale().x, DifficultyMode::paddleLengthScale });

	SetInitialDirection(true);
	SetDirection(GetInitialDirection());
	SetPosition(GetInitialPosition());

	auto capsule = dynamic_cast<BoundingCapsule<SFCapsule>*>(m_volume.get());
	capsule->Update(GetPosition());

	float radius = capsule->GetRadius();
	float capsuleHeight = capsule->GetLength();

	m_edgeBuffer = (radius + (capsuleHeight / 2.f)) + DifficultyMode::wallThickness;
}

void Player::Update(float deltaTime)
{
	ProcessInput();

	if (GetYVelocity() != 0)
	{
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
	}
}

void Player::Move(float x, float y)
{
	auto spr = dynamic_cast<SFSprite*>(m_drawable.get());
	if (spr)
		spr->Move(x, y);

	auto capsule = dynamic_cast<BoundingCapsule<SFCapsule>*>(m_volume.get());
	if (capsule)
		capsule->Update(GetPosition());
}

void Player::Move(const Vector2f& mov)
{
	Move(mov.x, mov.y);
}

bool Player::Intersects(IDynamicGameObject* obj, float& tFirst, float& tLast)
{
	bool col = false;
	if (DynamicGameObject::Intersects(obj, tFirst, tLast))
	{
		if (Ball* ball = dynamic_cast<Ball*>(obj))
		{
			if (m_collidedWithBall)
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

		if (m_collidedWithBall)
		{
			OnCollisionExit(obj);
			obj->OnCollisionExit(this);
		}
	}

	return col;
}

void Player::OnCollisionEnter(IGameObject* obj)
{
	SetCollisionWithBall(true);
}

void Player::OnCollisionStay(IGameObject* obj)
{
}

void Player::OnCollisionExit(IGameObject* obj)
{
	SetCollisionWithBall(false);
}

void Player::ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos)
{
	if (time < 1.0f)
	{
		// Step 1: Move player to point of impact
		Vector2f pos = GetPosition();
		Vector2f vel = GetVelocity();
		Vector2f impactOffset = vel * time;

		Vector2f collisionPos = pos + impactOffset;
		SetPosition(collisionPos.x, collisionPos.y);
		m_volume->Update(collisionPos);

		// Step 2: Apply minimum translation vector to separate
		if (sepVec != Vector2f(0.f, 0.f))
		{
			Vector2f resolvedPos = collisionPos + sepVec;
			SetPosition(resolvedPos.x, resolvedPos.y);
			m_volume->Update(resolvedPos);

			// Zero velocity in direction of sepVec
			if ((sepVec.y < 0 && vel.y < 0) || (sepVec.y > 0 && vel.y > 0))
			{
				SetYVelocity(0);
			}
		}
	}
}

void Player::SetCollisionWithBall(bool hasCollided)
{
	m_collidedWithBall = hasCollided;
}

void Player::Reset()
{
	DynamicGameObject::Reset();
	SetCollisionWithBall(false);
}

void Player::ProcessInput()
{
	auto gameMgr = GameManager::Get();
	auto inputManager = gameMgr->GetInputManager();

	if (inputManager->GetKeyState((int)m_keys[Keys::MoveUpKey]))
		SetYVelocity(-m_paddleSpeed);

	if (inputManager->GetKeyState((int)m_keys[Keys::MoveDownKey]))
		SetYVelocity(m_paddleSpeed);

	if (!inputManager->GetKeyState((int)m_keys[Keys::MoveUpKey]) && !inputManager->GetKeyState((int)m_keys[Keys::MoveDownKey]))
		SetYVelocity(0);

	if (m_collidedWithBall && !m_hasAppliedShot)
	{
		switch ((int)inputManager->GetFirstPressedKey(m_specialKeys))
		{
		case FastShotKey:
			BallPhysics::ApplyVelocityIncrease(DifficultyMode::speedFactor);
			break;
		case SlowShotKey:
			BallPhysics::ApplyVelocityDecrease(DifficultyMode::speedFactor);
			break;
		case TopSpinKey:
			BallPhysics::ApplyTopSpin(DifficultyMode::speedFactor);
			break;
		case BackSpinKey:
			BallPhysics::ApplyBackSpin(DifficultyMode::speedFactor);
			break;
		case LeftSpinKey:
			BallPhysics::ApplyLeftSpin(DifficultyMode::speedFactor);
			break;
		case RightSpinKey:
			BallPhysics::ApplyRightSpin(DifficultyMode::speedFactor);
			break;
		}

		m_hasAppliedShot = true;
	}

	/*if (inputManager->GetKeyState((int)KeyCode::Space))
		gameMgr->GetGameStateMgr()->PushState(new PauseMenuState(gameMgr));*/
}

AutomatedPlayer::AutomatedPlayer(PlayerIdentifiers plyID)
	: Player(plyID), m_aiCtrl(DifficultyMode::difficulty), m_gen(m_rd())
{
	switch (DifficultyMode::difficulty)
	{
	case Easy:
		m_bufferTime = 10.f;
		break;
	case Normal:
		m_bufferTime = 5.f;
		break;
	case Hard:
		m_bufferTime = 2.f;
	}

	m_reactionDelay = GenerateReactionDelay();

	m_targetY = GetPosition().y;
}

void AutomatedPlayer::Update(float deltaTime)
{
	m_opponent = GetOpponent();

	auto scene = GameManager::Get()->GetScene();
	if (!scene)
		return;

	const Vector2f& ballVel = BallPhysics::GetVelocity();

	if (!ShouldReactToBall(ballVel))
		return;

	m_reactionTimer += deltaTime;

	const Vector2f& paddlePos = GetPosition();

	if (m_reactionTimer >= m_reactionDelay)
		PredictBallTarget(paddlePos, ballVel);

	UpdateMovement(paddlePos, deltaTime);
	ApplyPendingShot();
}


void AutomatedPlayer::SetCollisionWithBall(bool hasCollided)
{
	// new collision detected
	if (hasCollided && !HasCollidedWithBall())
		m_hasAppliedShot = false;

	Player::SetCollisionWithBall(hasCollided);
}

void AutomatedPlayer::UpdateFatigue(int rallyCount)
{
	m_fatigue = rallyCount * 0.4f;
}

void AutomatedPlayer::UpdateStress(int myScore, int oppScore)
{
	m_stress = (myScore - oppScore) * 0.6f;
}

bool AutomatedPlayer::Intersects(IGameObject* obj)
{
	return false;
}

void AutomatedPlayer::Reset()
{
	m_fatigue = 0;
	Player::Reset();
}

bool AutomatedPlayer::ShouldReactToBall(const Vector2f& ballVel)
{
	PlayerIdentifiers id = GetPlayerID();

	return !((id == Player1 && ballVel.x > 0) || (id == Player2 && ballVel.x < 0));
}

Player* AutomatedPlayer::GetOpponent()
{
	switch (GetPlayerID())
	{
	case Player1:
		return static_cast<Player*>(GameManager::Get()->GetScene()->GetObjectByName("SecondPlayer"));
	case Player2:
		return static_cast<Player*>(GameManager::Get()->GetScene()->GetObjectByName("FirstPlayer"));
	}

	return nullptr;
}

bool AutomatedPlayer::BiasedRandomChoice(float biasFactor, bool& lastResult)
{
	if ((std::rand() % 100) < (biasFactor * 100))
	{
		// Keep last result with probability `biasFactor`
		return lastResult;
	}
	else
	{
		// Switch result
		lastResult = !lastResult;
		return lastResult;
	}
}

float AutomatedPlayer::StandardShot()
{
	// Generate a small random offset within a safe range (-5 to 5)
	static std::uniform_real_distribution<float> offsetDist(-7.50f, 7.5f);
	return offsetDist(m_gen);
}

float AutomatedPlayer::AggressiveShot()
{
	float oppY = m_opponent->GetPosition().y;
	float screenCenter = GameConstants::ScreenDim.y / 2.0f;

	// Increase ball speed aggressively
	BallPhysics::ApplyVelocityIncrease(1.1f);

	// Calculate how far the opponent is from the center
	float opponentOffset = oppY - screenCenter;

	// Set aggressive shot target based on opponent position
	float attackOffset = 15.0f; // Default aggressive offset to target edges

	if (std::abs(opponentOffset) > (GameConstants::ScreenDim.y * 0.1f)) // If opponent is significantly off-center
	{
		attackOffset = (opponentOffset > 0) ? -10.0f : 10.0f; // If opponent is high, aim low, and vice versa
	}

	// Add slight randomness to avoid predictable shots
	static std::uniform_real_distribution<float> randomDist(-3.0f, 3.0f);
	attackOffset += randomDist(m_gen);

	return attackOffset;
}

float AutomatedPlayer::DefensiveShot()
{
	float ballSpeed = std::abs(BallPhysics::GetVelocity().x);
	float oppY = m_opponent->GetPosition().y;

	// Reduce ball speed to make it easier to return
	BallPhysics::ApplyVelocityDecrease(0.9f);

	// Defensive positioning: aim for the middle of the paddle
	float safeOffset = 5.0f;

	// If opponent is near top, aim lower; if opponent is near bottom, aim higher
	if (oppY < GameConstants::ScreenDim.y / 3.0f)
		safeOffset = -5.0f;
	else if (oppY > (2 * GameConstants::ScreenDim.y) / 3.0f)
		safeOffset = 5.0f;

	// Limit the offset to a reasonable range
	return std::clamp(safeOffset, -7.0f, 7.0f);
}

float AutomatedPlayer::FastShot()
{
	if (m_pendingShot != ShotTypes::FastShot)
		m_pendingShot = ShotTypes::FastShot;

	return StandardShot();
}

float AutomatedPlayer::SlowShot()
{
	if (m_pendingShot != ShotTypes::SlowShot)
		m_pendingShot = ShotTypes::SlowShot;

	return StandardShot();
}

float AutomatedPlayer::ChangeUpShot()
{
	if (BiasedRandomChoice(DifficultyMode::bias, m_lastSpeedShot))
		return FastShot();
	else
		return SlowShot();
}

float AutomatedPlayer::TopSpinShot()
{
	if (m_pendingShot != ShotTypes::TopSpinShot)
		m_pendingShot = ShotTypes::TopSpinShot;

	return StandardShot();
}

float AutomatedPlayer::BackSpinShot()
{
	if (m_pendingShot != ShotTypes::BackSpinShot)
		m_pendingShot = ShotTypes::BackSpinShot;

	return StandardShot();
}

float AutomatedPlayer::LengthSpinShot()
{
	if (BiasedRandomChoice(DifficultyMode::bias, m_lastLengthSpin))
		return TopSpinShot();
	else
		return BackSpinShot();
}

float AutomatedPlayer::LeftSpinShot()
{
	if (m_pendingShot != ShotTypes::LeftSpinShot)
		m_pendingShot = ShotTypes::LeftSpinShot;

	return StandardShot();
}

float AutomatedPlayer::RightSpinShot()
{
	if (m_pendingShot != ShotTypes::RightSpinShot)
		m_pendingShot = ShotTypes::RightSpinShot;

	return StandardShot();
}

float AutomatedPlayer::WidthSpinShot()
{
	if (BiasedRandomChoice(DifficultyMode::bias, m_lastWidthSpin))
		return LeftSpinShot();
	else
		return RightSpinShot();
}

float AutomatedPlayer::FakeShot(int playstyle)
{
	// Calculate paddle move distance needed to reach predicted ball position
	float paddleMoveDistance = std::abs(GetPosition().y - m_predictedY);

	// Calculate the last possible moment to move
	float lastMoveTime = m_timeToPaddle - (paddleMoveDistance / DifficultyMode::paddleSpeed);

	m_timeDelay = std::max(0.1f, std::min(0.5f, lastMoveTime)); // Ensure it's not negative

	float shotOffset = 0;

	switch (playstyle)
	{
	case Balance:
		shotOffset = StandardShot();
		break;
	case Offense:
		shotOffset = AggressiveShot();
		break;
	case Defense:
		shotOffset = DefensiveShot();
		break;
	}

	return shotOffset;
}

float AutomatedPlayer::CornerShot()
{
	float oppY = m_opponent->GetPosition().y;

	float targetY = (oppY < GameConstants::ScreenDim.y / 2.0f) ?
		GameConstants::ScreenDim.y - 10.0f :
		GameConstants::ScreenDim.y - 10.0f;

	float shotOffset = targetY - BallPhysics::GetPosition().y;

	// Limit the offset to prevent unrealistic movement
	return std::clamp(shotOffset, -15.0f, 15.0f);
}

float AutomatedPlayer::WallShot()
{
	const auto& ballPos = BallPhysics::GetPosition();
	const auto& ballVel = BallPhysics::GetVelocity();

	// Predict time for ball to reach opponent paddle
	float timeToOpponent = (m_opponent->GetPosition().x - ballPos.x) / ballVel.x;
	float predictedBallY = ballPos.y + (ballVel.y * timeToOpponent);

	// Check if ball will hit the top or bottom wall
	if (predictedBallY < 0.0f || predictedBallY > GameConstants::ScreenDim.y)
	{
		// Set bounce position as close as possible to the wall before reaching the opponent
		float latestBounceY = (ballVel.y > 0) ? (GameConstants::ScreenDim.y - m_bufferTime) : m_bufferTime;

		// Compute shot offset needed to achieve this bounce
		return latestBounceY - predictedBallY;
	}

	return 0.0f;
}

float AutomatedPlayer::EdgeShot()
{
	static std::uniform_real_distribution<float> edgeDist(0.2f, 0.8f);
	float edgeFactor = edgeDist(m_gen);
	return (edgeFactor * (m_edgeBuffer * 2.0f)) - m_edgeBuffer;
}

float AutomatedPlayer::GetShotOffset()
{
	PlayStyles playStyle = (PlayStyles)m_aiCtrl.GetPlayStyle();

	m_timeDelay = 0;

	float shotOffSet = 0;
	m_pendingShot = m_aiCtrl.GetShotType(playStyle);
	m_shotReady = true;
	m_hasAppliedShot = false;

	switch (m_pendingShot)
	{
	case ShotTypes::StraightShot:
		// no offset
		break;
	case ShotTypes::StandardShot:
		shotOffSet = StandardShot();
		break;
	case ShotTypes::AggresiveShot:
		shotOffSet = AggressiveShot();
		break;
	case ShotTypes::DefensiveShot:
		shotOffSet = DefensiveShot();
		break;
	case ShotTypes::FastShot:
		shotOffSet = FastShot();
		break;
	case ShotTypes::SlowShot:
		shotOffSet = SlowShot();
		break;
	case ShotTypes::ChangeUpShot:
		shotOffSet = ChangeUpShot();
		break;
	case ShotTypes::TopSpinShot:
		shotOffSet = TopSpinShot();
		break;
	case ShotTypes::BackSpinShot:
		shotOffSet = BackSpinShot();
		break;
	case ShotTypes::LengthSpinShot:
		shotOffSet = LengthSpinShot();
		break;
	case ShotTypes::LeftSpinShot:
		shotOffSet = LeftSpinShot();
		break;
	case ShotTypes::RightSpinShot:
		shotOffSet = RightSpinShot();
		break;
	case ShotTypes::WidthSpinShot:
		shotOffSet = WidthSpinShot();
		break;
	case ShotTypes::FakeShot:
		shotOffSet = FakeShot(playStyle);
		break;
	case ShotTypes::CornerShot:
		shotOffSet = CornerShot();
		break;
	case ShotTypes::WallShot:
		shotOffSet = WallShot();
		break;
	}

	return shotOffSet;
}

float AutomatedPlayer::GenerateReactionDelay()
{
	std::uniform_real_distribution<float> dist(DifficultyMode::reactionDelay.first, DifficultyMode::reactionDelay.second);
	return dist(m_gen);
}

void AutomatedPlayer::PredictBallTarget(const Vector2f& paddlePos, const Vector2f& ballVel)
{
	const Vector2f ballPos = BallPhysics::GetPosition();

	if (std::abs(ballVel.x) < 1e-6f)
		return;

	float miscalculationFactor = 1.0f;
	static std::uniform_real_distribution<float> oddsDist(0.0f, 1.0f);
	if (oddsDist(m_gen) < 0.05f)
	{
		static std::uniform_int_distribution<int> signDist(0, 1);
		float adjustment = 0.1f * (signDist(m_gen) == 0 ? 1.0f : -1.0f);
		miscalculationFactor += adjustment;
	}

	m_timeToPaddle = ((paddlePos.x - ballPos.x) / ballVel.x) * miscalculationFactor;
	m_predictedY = ballPos.y + ballVel.y * m_timeToPaddle;

	const float screenHeight = GameConstants::ScreenDim.y;
	m_predictedY = std::fmod(m_predictedY + 2 * screenHeight, 2 * screenHeight);
	if (m_predictedY > screenHeight)
		m_predictedY = 2 * screenHeight - m_predictedY;

	std::uniform_real_distribution<float> dist(-DifficultyMode::errorVal, DifficultyMode::errorVal);
	float errorOffset = dist(m_gen) * (m_stress + m_fatigue);

	m_targetY = m_predictedY + errorOffset + GetShotOffset();

	m_reactionTimer = 0.0f;
	m_reactionDelay = GenerateReactionDelay();
}

void AutomatedPlayer::UpdateMovement(const Vector2f& paddlePos, float deltaTime)
{
	const float speedAdjustment = 0.5f;
	const float maxSpeed = DifficultyMode::paddleSpeed;
	const float diff = m_targetY - paddlePos.y;

	m_timeDelay -= deltaTime;
	if (m_timeDelay >= 0)
		return;

	if (std::abs(diff) > 1.5f)
	{
		float newVelocity = GetYVelocity() + std::copysign(speedAdjustment, diff);
		newVelocity = std::clamp(newVelocity, -maxSpeed, maxSpeed);
		SetYVelocity(newVelocity);
		GameManager::Get()->GetCollisionMgr()->ProcessCollisions(this);
		Move(0, GetYVelocity() * GameConstants::FPS * deltaTime);
	}
	else
	{
		SetYVelocity(GetYVelocity() * 0.8f);
		if (std::abs(GetYVelocity()) < 0.1f)
			SetYVelocity(0);
	}

	m_timeDelay = 0;
}

void AutomatedPlayer::ApplyPendingShot()
{
	if (!HasCollidedWithBall() || !m_shotReady || m_hasAppliedShot)
		return;

	switch (m_pendingShot)
	{
	case ShotTypes::FastShot:
		BallPhysics::ApplyVelocityIncrease(DifficultyMode::speedFactor);
		break;
	case ShotTypes::SlowShot:
		BallPhysics::ApplyVelocityDecrease(DifficultyMode::speedFactor);
		break;
	case ShotTypes::TopSpinShot:
		BallPhysics::ApplyTopSpin(DifficultyMode::speedFactor);
		break;
	case ShotTypes::BackSpinShot:
		BallPhysics::ApplyBackSpin(DifficultyMode::speedFactor);
		break;
	case ShotTypes::LeftSpinShot:
		BallPhysics::ApplyLeftSpin(DifficultyMode::speedFactor);
		break;
	case ShotTypes::RightSpinShot:
		BallPhysics::ApplyRightSpin(DifficultyMode::speedFactor);
		break;
	default:
		break;
	}

	m_hasAppliedShot = true;
	m_shotReady = false;
}