#pragma once

#include <map>
#include <utility>

enum Difficulty
{
	Easy,
	Normal,
	Hard
};

class DifficultyMode
{
public:
	// Game Difficulty & Configuration
	static Difficulty difficulty;					// Defines AI difficulty level

	// AI Reaction & Error Variables
	static float errorVal;							// AI error margin for human-like play
	static std::pair<float, float> reactionDelay;	// Min/Max reaction delay range

	// Paddle & Movement Speed Control
	static float paddleLengthScale;					// paddle length scale
	static float paddleSpeed;						// Paddle movement speed
	static float speedFactor;						// Ball speed multiplier

	// AI Decision Bias & Game Rules
	static float bias;								// AI decision-making bias
	static int rallieLimit;							// Max number of ball rallies before AI switches strategies

	static float wallThickness;						// The thickness of the surrounding walls of the court

	static void EasyMode();
	static void MediumMode();
	static void HardMode();
};