#pragma once

#include <Engine/AI/DecisionTree.h>

enum GameStates : uint64_t
{
	// even states
	FreshStart = 0x0,						// The game just started (0-0).
	DeadEven = 0x1,							// Players are at the exact same score (2-2, 5-5, 8-8).
	NeckAndNeck = 0x2,						// Players are within 1 point of each other, going back and forth (3-2, 6-5, 8-7).

	// lead states
	IsLeading = 0x4,						// This player is leading
	JustAhead = 0x8,						// Leading by 1 point (3-2, 5-4, 8-7).
	NarrowLead = 0x10,						// Leading by 2 points (4-2, 7-5).
	SlimLead = 0x20,						// Leading by 3 points (6-3, 8-5).
	HoldingTheLead = 0x40,					// Maintaining a steady 3+ point lead (7-4, 8-5).
	SolidLead = 0x80,						// Leading by 4 points (7-3, 8-4).
	CommandingLead = 0x100,					// Leading by 5 points (6-1, 8-3).
	RunawayLead = 0x200,					// Leading by 6 points (7-1, 8-2).
	BlowoutMargin = 0x400,					// Leading by 7 points (8-1).
	UnassailableLead = 0x800,				// Leading by 8 points, one point from winning (8-0).

	// trail states
	IsTrailing = 0x1000,					// This player is trailing
	JustBehind = 0x2000,					// Trailing by 1 point (2-3, 5-6, 7-8).
	NarrowDeficit = 0x4000,					// Trailing by 2 points (2-4, 5-7).
	SlimDeficit = 0x8000,					// Trailing by 3 points (3-6, 5-8).
	StrugglingToCatchUp = 0x10000,			// Trailing by 3+ points, showing no clear recovery.
	ModerateDeficit = 0x20000,				// Trailing by 4 points (3-7, 4-8).
	SignificantlyBehind = 0x40000,			// Trailing by 5 points (2-7, 3-8).
	FallingTooFarBehind = 0x80000,			// Trailing by 6 points (2-8, 1-7).
	GettingCrushed = 0x100000,				// Trailing by 7 points (1-8).
	HopelessDeficit = 0x200000,				// Trailing by 8 points, about to lose (0-8).

	// momentum states
	ComebackInProgress = 0x400000,			// The losing player was behind by 3+ points but is now closing in.
	StalledLead = 0x800000,					// The leading player struggles to finish the game, losing multiple points in a row.
	UnderPressure = 0x1000000,				// The leader is on the back foot as the opponent gains momentum.

	// pressure states
	LosingMomentum = 0x2000000,				// A leading player is losing their advantage quickly.
	ClosingTheGap = 0x4000000,				// The trailing player is rapidly reducing the score gap.
	SmellingBlood = 0x8000000,				// The trailing player is within 1 point of tying or taking the lead.

	// momentum/pressure states
	TippingPoint = 0x10000000,				// A pivotal moment where either player could take control.

	// critical states
	NonCritical = 0x20000000,
	MatchPoint = 0x40000000,				// The leading player is one point away from winning.
	FacingElimination = 0x80000000,			// The losing player must win the next point or lose the game.
	SuddenDeath = 0x100000000,				// The next point determines the winner.
	GamePointSaved = 0x200000000,			// The losing player prevents the opponent from winning at match point.
	FailedToCloseTheGame = 0x4000000000,	// A player who was at match point but lost multiple points in a row.
};

enum PlayerIdentifiers;

class GameStateEvaluator : public DecisionTree<GameStates, bool, int>
{
public:
	GameStateEvaluator();
	~GameStateEvaluator() = default;

	void SetCurrentAnalyser(PlayerIdentifiers id) { m_currentAnalyser = id; }

	static void AddNewGoalHistoryEntry(PlayerIdentifiers id);

private:

	void AddStandingBranch();
	void AddPerformanceBranch();
	void AddCriticalBranch();

	// Standing Conditions
	static bool IsTied(int standing) { return standing == 0; }
	static bool IsEven() { return m_goalHistory.size() > 0; }
	static bool IsWinning(int standing) { return standing > 0; }
	static bool IsJustAhead(int standing) { return abs(standing) == 1; }
	static bool IsNarrowLead(int standing) { return abs(standing) == 2; }
	static bool IsSlimLead(int standing) { return abs(standing) == 3; }
	static bool IsHoldingTheLead(int standing) { return abs(standing) > 3; }
	static bool IsSolidLead(int standing) { return abs(standing) == 4; }
	static bool IsCommandingLead(int standing) { return abs(standing) == 5; }
	static bool IsRunawayLead(int standing) { return abs(standing) == 6; }
	static bool IsBlowoutMargin(int standing) { return abs(standing) == 7; }
	static bool IsUnassailableLead(int standing) { return abs(standing) == 8; }

	// Performance Conditions
	static bool IsNeckAndNeck(int standing);

		// Pressure
	static bool IsAStalledLead();
	static bool IsLosingMomentum();
	static bool IsUnderPressure();

		// Momentum
	static bool IsComebackInProgress();
	static bool IsClosingTheGap();
	static bool IsSmellingBlood();

	// Critical Conditions
	static bool IsAtMatchPoint();
	static bool HasFailedToCloseTheGame();

	static bool IsFacingElimination();
	static bool HasSavedTheGame();

	static bool IsInSuddenDeath();

	// Helper for conditional functions
	static int GetScoreStreak(bool mine);
	static int GetMyScore(int excludeLastN = 0);
	static int GetOpponentScore(int excludeLastN = 0);
	static bool HadDominantLeadBefore(bool mine, int streak);
	static bool HadDominantLeadBefore(bool mine, int streak, int& scoreToBeChecked, int& otherScore);

	static PlayerIdentifiers m_currentAnalyser;
	static std::vector<PlayerIdentifiers> m_goalHistory;
};