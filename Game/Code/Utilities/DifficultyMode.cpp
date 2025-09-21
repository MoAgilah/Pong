#include "DifficultyMode.h"

#include "../Engine/AI/PongAICtrl.h"
#include <Engine/Core/Constants.h>

Difficulty DifficultyMode::difficulty = Normal;
float DifficultyMode::errorVal = 0.f;
std::pair<float,float> DifficultyMode::reactionDelay = std::make_pair(0.1f,0.4f);
float DifficultyMode::paddleLengthScale = 1.f;
float DifficultyMode::paddleSpeed = 0.f;
float DifficultyMode::speedFactor = 1.2f;
int DifficultyMode::rallieLimit = 5;
float DifficultyMode::bias = 0.5;
float DifficultyMode::wallThickness = 24.f;

void DifficultyMode::EasyMode()
{
	difficulty = Easy;

	errorVal = 15.0f;
	reactionDelay = std::make_pair(0.3f, 0.5f);
	paddleLengthScale = 1.2f;
	paddleSpeed = GameConstants::ObjectSpeed * 0.6f;
	speedFactor = 1.1f;
	rallieLimit = 9;
	bias = 0.7f;
}

void DifficultyMode::MediumMode()
{
	difficulty = Normal;

	errorVal = 7.5f;
	paddleSpeed = GameConstants::ObjectSpeed * 0.8f;
}

void DifficultyMode::HardMode()
{
	difficulty = Hard;

	errorVal = 1.5f;
	reactionDelay = std::make_pair(0.05f, 0.15f);
	paddleSpeed = GameConstants::ObjectSpeed * 1.1f;
	paddleLengthScale = 0.7f;
	speedFactor = 1.2f;
	rallieLimit = 3;
	bias = 0.3f;
}