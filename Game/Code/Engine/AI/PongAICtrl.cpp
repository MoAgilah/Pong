#include "PongAICtrl.h"

#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/MatchCtrl.h"
#include <algorithm>
#include <vector>

std::vector<double> rigidDistribution = { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
std::vector<double> balanceDistribution = { 0.2, 0.16, 0.16, 0.16, 0.16, 0.16 };
std::vector<double> maxVarietyDistribution = { 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0 };

PongAICtrl::PongAICtrl(Difficulty difficulty)
	: m_gen(m_rd())
{
	switch (difficulty)
	{
	case Easy:
		m_playStyles = { {Balance, 0.5}, {Offense, 0.2}, {Defense, 0.3} };
		break;
	case Normal:
		m_playStyles = { {Balance, 0.33}, {Offense, 0.33}, {Defense, 0.33} };
		break;
	case Hard:
		m_playStyles = { {Balance, 0.3}, {Offense, 0.4}, {Defense, 0.3} };
		break;
	}

	m_balancedShots = { {StraightShot, 0.2}, {StandardShot, 0.16}, {ChangeUpShot, 0.16}, {LengthSpinShot, 0.16}, {WidthSpinShot, 0.16}, {FakeShot, 0.16}, };
	m_offensiveShots = { {AggresiveShot, 0.2}, {FastShot, 0.16}, {TopSpinShot, 0.16}, {LeftSpinShot, 0.16}, {RightSpinShot, 0.16}, {CornerShot, 0.16}, };
	m_defensiveShots = { {DefensiveShot, 0.2}, {SlowShot, 0.16}, {BackSpinShot, 0.16}, {WallShot, 0.16}, {WidthSpinShot, 0.16}, {ChangeUpShot, 0.16}, };
}

int PongAICtrl::GetPlayStyle()
{
	ChangeStrategy();

	return GetWeightedChoice(m_playStyles);
}

int PongAICtrl::GetShotType(int playStyle)
{
	int choice = 0;

	switch (playStyle)
	{
	case Balance:
		choice = GetWeightedChoice(m_balancedShots);
		break;
	case Offense:
		choice = GetWeightedChoice(m_offensiveShots);
		break;
	case Defense:
		choice = GetWeightedChoice(m_defensiveShots);
		break;
	}

	return choice;
}

void PongAICtrl::ChangeStrategy()
{
	if (PongAICtrl::m_playStyles.empty())
		throw std::runtime_error("No play types available.");

	auto flags = MatchCtrl::GetCurrentMatchResults();

	if ((flags & m_previousGameState) == m_previousGameState)
		return;

	m_previousGameState = flags;

	double balanceMod = 0;
	double AggressiveMod = 0;
	double DefensiveMod = 0;

	int cnt = 0;

	while (flags)
	{
		GameStates flag = GameStates(flags & -flags); // Extract the lowest set bit

		switch (flag)
		{
		case FreshStart:
			balanceMod += 0.05; AggressiveMod -= 0.02; DefensiveMod -= 0.03;
			break;
		case DeadEven:
			balanceMod += 0.03; AggressiveMod -= 0.02; DefensiveMod -= 0.01;
			break;
		case NeckAndNeck:
			balanceMod += 0.02; AggressiveMod += 0.01; DefensiveMod -= 0.03;
			break;
		case IsLeading:
			balanceMod -= 0.02; AggressiveMod += 0.02;
			break;
		case JustAhead:
			balanceMod -= 0.03; AggressiveMod += 0.01; DefensiveMod += 0.02;
			break;
		case NarrowLead:
			balanceMod -= 0.04; AggressiveMod += 0.01; DefensiveMod += 0.05;
			break;
		case SlimLead:
			balanceMod -= 0.06; AggressiveMod -= 0.02; DefensiveMod += 0.08;
			break;
		case HoldingTheLead:
			balanceMod -= 0.08; AggressiveMod -= 0.03; DefensiveMod += 0.11;
			break;
		case SolidLead:
			balanceMod -= 0.1; AggressiveMod -= 0.04; DefensiveMod -= 0.14;
			break;
		case CommandingLead:
			balanceMod -= 0.12; AggressiveMod -= 0.06; DefensiveMod += 0.18;
			break;
		case RunawayLead:
			balanceMod -= 0.15; AggressiveMod -= 0.08; DefensiveMod += 0.22;
			break;
		case BlowoutMargin:
			balanceMod -= 0.18; AggressiveMod -= 0.08; DefensiveMod += 0.26;
			break;
		case UnassailableLead:
			balanceMod -= 0.2; AggressiveMod -= 0.1; DefensiveMod += 0.3;
			break;
		case IsTrailing:
			balanceMod -= 0.02; AggressiveMod += 0.02;
			break;
		case JustBehind:
			balanceMod -= 0.03; AggressiveMod += 0.04; DefensiveMod -= 0.01;
			break;
		case NarrowDeficit:
			balanceMod -= 0.04; AggressiveMod += 0.06; DefensiveMod -= 0.01;
			break;
		case SlimDeficit:
			balanceMod -= 0.05; AggressiveMod += 0.08; DefensiveMod -= 0.02;
			break;
		case StrugglingToCatchUp:
			balanceMod -= 0.06; AggressiveMod += 0.1; DefensiveMod -= 0.04;
			break;
		case ModerateDeficit:
			balanceMod -= 0.08; AggressiveMod += 0.12; DefensiveMod -= 0.04;
			break;
		case SignificantlyBehind:
			balanceMod -= 0.1; AggressiveMod += 0.15; DefensiveMod -= 0.05;
			break;
		case FallingTooFarBehind:
			balanceMod -= 0.12; AggressiveMod += 0.18; DefensiveMod -= 0.06;
			break;
		case GettingCrushed:
			balanceMod -= 0.15; AggressiveMod += 0.2; DefensiveMod -= 0.08;
			break;
		case HopelessDeficit:
			balanceMod -= 0.18; AggressiveMod += 0.25; DefensiveMod -= 0.07;
			break;
		case ComebackInProgress:
			balanceMod -= 0.05; AggressiveMod += 0.12; DefensiveMod -= 0.07;
			break;
		case StalledLead:
			balanceMod -= 0.07; AggressiveMod -= 0.03; DefensiveMod += 0.1;
			break;
		case UnderPressure:
			balanceMod -= 0.08; AggressiveMod -= 0.02; DefensiveMod += 0.1;
			break;
		case LosingMomentum:
			balanceMod -= 0.1; AggressiveMod -= 0.05; DefensiveMod += 0.15;
			break;
		case ClosingTheGap:
			balanceMod -= 0.05; AggressiveMod += 0.14; DefensiveMod -= 0.09;
			break;
		case SmellingBlood:
			balanceMod -= 0.05; AggressiveMod += 0.18; DefensiveMod -= 0.11;
			break;
		case TippingPoint:
			balanceMod -= 0.05; AggressiveMod += 0.2; DefensiveMod -= 0.15;
			break;
		case NonCritical:
			balanceMod += 0.02; AggressiveMod -= 0.01; DefensiveMod -= 0.01;
			break;
		case MatchPoint:
			balanceMod -= 0.1; AggressiveMod -= 0.15; DefensiveMod += 0.25;
			break;
		case FacingElimination:
			balanceMod -= 0.08; AggressiveMod += 0.25; DefensiveMod -= 0.17;
			break;
		case SuddenDeath:
			balanceMod -= 0.1; AggressiveMod += 0.3; DefensiveMod -= 0.2;
			break;
		case GamePointSaved:
			balanceMod -= 0.07; AggressiveMod += 0.2; DefensiveMod -= 0.13;
			break;
		case FailedToCloseTheGame:
			balanceMod -= 0.09; AggressiveMod += 0.18; DefensiveMod -= 0.09;
			break;
		}

		flags &= (flags - 1); // Remove the lowest set bit
		cnt++;
	}

	// Apply modifications
	double balanced = m_playStyles[Balance] + balanceMod;
	double aggressive = m_playStyles[Offense] + AggressiveMod;
	double defensive = m_playStyles[Defense] + DefensiveMod;

	// Normalize (ensure total = 1)
	double total = balanced + aggressive + defensive;
	balanced /= total;
	aggressive /= total;
	defensive /= total;

	m_playStyles[Balance] = std::max(0.0, balanced);
	m_playStyles[Offense] = std::max(0.0, aggressive);
	m_playStyles[Defense] = std::max(0.0, defensive);

	ChangeShotDistribution(m_playStyles[Balance], m_balancedShots);
	ChangeShotDistribution(m_playStyles[Offense], m_offensiveShots);
	ChangeShotDistribution(m_playStyles[Defense], m_defensiveShots);
}

void PongAICtrl::ChangeShotDistribution(double playStyleWeight, std::map<ShotTypes, double>& shots)
{
	std::vector<double> distribution(shots.size());
	if (playStyleWeight < 0.5f)
	{
		double factor = playStyleWeight / 0.5;
		for (size_t i = 0; i < shots.size(); i++)
		{
			distribution[i] = std::lerp(rigidDistribution[i], balanceDistribution[i], factor);
		}
	}
	else
	{
		double factor = (playStyleWeight - 0.5) / 0.5;
		for (size_t i = 0; i < shots.size(); i++)
		{
			distribution[i] = std::lerp(balanceDistribution[i], maxVarietyDistribution[i], factor);
		}
	}

	int cnt = 0;
	for (auto& [_, weight] : shots)
		weight = distribution[cnt++];
}