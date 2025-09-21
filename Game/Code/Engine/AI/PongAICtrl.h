#pragma once

#include <map>
#include <random>

enum PlayStyles
{
	Balance, Offense, Defense
};

enum ShotTypes
{
	StraightShot,
	StandardShot,
	AggresiveShot,
	DefensiveShot,
	FastShot,
	SlowShot,
	ChangeUpShot,
	TopSpinShot,
	BackSpinShot,
	LengthSpinShot,
	LeftSpinShot,
	RightSpinShot,
	WidthSpinShot,
	FakeShot,
	CornerShot,
	WallShot
};

enum Difficulty;

class PongAICtrl
{
public:
	PongAICtrl(Difficulty difficulty);

	int GetPlayStyle();
	int GetShotType(int playStyle);

private:

	template<typename Key>
	int GetWeightedChoice(const std::map<Key, double>& container);
	void ChangeStrategy();
	void ChangeShotDistribution(double playStyleWeight, std::map<ShotTypes, double>& shots);

	// Game State Tracking
	int m_previousGameState = 0;

	// Random Number Generation
	std::random_device m_rd;
	std::mt19937 m_gen;

	// AI Playstyle & Shot Selection Probabilities
	std::map<PlayStyles, double> m_playStyles;      // AI playstyle preference mapping
	std::map<ShotTypes, double> m_balancedShots;    // Balanced shot type probabilities
	std::map<ShotTypes, double> m_offensiveShots;   // Offensive shot type probabilities
	std::map<ShotTypes, double> m_defensiveShots;   // Defensive shot type probabilities
};

template<typename Key>
inline int PongAICtrl::GetWeightedChoice(const std::map<Key, double>& container)
{
	// Ensure there are play types to choose from
	if (container.empty())
		throw std::invalid_argument("GetWeightedChoice: container is empty.");

	// Prepare weight distribution
	std::vector<Key> keys;
	std::vector<double> weights;

	for (const auto& pair : container)
	{
		keys.push_back(pair.first);
		weights.push_back(pair.second);
	}

	// Weighted random selection
	std::discrete_distribution<int> distribution(weights.begin(), weights.end());

	// Ensure m_gen is initialized properly (should be defined in PongAICtrl)
	return keys[distribution(m_gen)];
}