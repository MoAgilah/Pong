#include "GameStateEvaluator.h"

#include "../../GameObjects/Player.h"
#include <Utilities/Utils.h>
#include <algorithm>
#include <ranges>


PlayerIdentifiers GameStateEvaluator::m_currentAnalyser = PlayerIdentifiers::Player1;
std::vector<PlayerIdentifiers> GameStateEvaluator::m_goalHistory{};

GameStateEvaluator::GameStateEvaluator()
	: DecisionTree<GameStates, bool, int>(std::make_shared<DecisionNode<GameStates, bool, int>>(&GameStateEvaluator::IsTied, GameStates::FreshStart))
{
	AddStandingBranch();
	AddPerformanceBranch();
	AddCriticalBranch();
}

void GameStateEvaluator::AddNewGoalHistoryEntry(PlayerIdentifiers id)
{
	m_goalHistory.push_back(id);
}

void GameStateEvaluator::AddStandingBranch()
{
	DECL_GET_OR_RETURN(root, GetRoot());

	auto ResetNode = std::make_shared<DecisionNode<GameStates, bool, int>>(
		[](int _) { return true; }, GameStates::FreshStart);

	DECL_GET_OR_RETURN(IsEven, AddNode(root, std::bind(&GameStateEvaluator::IsEven), GameStates::DeadEven, true));
	IsEven->m_false = ResetNode;

	DECL_GET_OR_RETURN(NotTied,AddNode(root, &GameStateEvaluator::IsWinning, GameStates::IsLeading, false));

	DECL_GET_OR_RETURN(IsLeading, AddNode(NotTied, &GameStateEvaluator::IsJustAhead, GameStates::JustAhead, true));
	DECL_GET_OR_RETURN(NotJustAhead, AddNode(IsLeading, &GameStateEvaluator::IsNarrowLead, GameStates::NarrowLead, false));
	DECL_GET_OR_RETURN(NotSlimLead, AddNode(NotJustAhead, &GameStateEvaluator::IsSlimLead, GameStates::SlimLead, false));
	DECL_GET_OR_RETURN(NotSolidLead, AddNode(NotSlimLead, &GameStateEvaluator::IsSolidLead, GameStates::SolidLead, false));
	DECL_GET_OR_RETURN(NotCommandingLead, AddNode(NotSolidLead, &GameStateEvaluator::IsCommandingLead, GameStates::CommandingLead, false));
	DECL_GET_OR_RETURN(NotRunawayLead, AddNode(NotCommandingLead, &GameStateEvaluator::IsRunawayLead, GameStates::RunawayLead, false));
	DECL_GET_OR_RETURN(NotBlowOutMargin, AddNode(NotRunawayLead, &GameStateEvaluator::IsBlowoutMargin, GameStates::UnassailableLead, false));

	DECL_GET_OR_RETURN(IsTrailing, AddNode(NotTied, &GameStateEvaluator::IsJustAhead, GameStates::JustBehind, false));
	DECL_GET_OR_RETURN(NotNarrowDeficit, AddNode(IsTrailing, &GameStateEvaluator::IsNarrowLead, GameStates::NarrowDeficit, false));
	DECL_GET_OR_RETURN(NotModerateDeficit, AddNode(NotNarrowDeficit, &GameStateEvaluator::IsSlimLead, GameStates::ModerateDeficit, false));
	DECL_GET_OR_RETURN(NotSignificantlyBehind, AddNode(NotModerateDeficit, &GameStateEvaluator::IsSolidLead, GameStates::SignificantlyBehind, false));
	DECL_GET_OR_RETURN(NotFallingTooFarBehind, AddNode(NotSignificantlyBehind, &GameStateEvaluator::IsCommandingLead, GameStates::FallingTooFarBehind, false));
	DECL_GET_OR_RETURN(NotGettingCrushed, AddNode(NotFallingTooFarBehind, &GameStateEvaluator::IsRunawayLead, GameStates::GettingCrushed, false));
	DECL_GET_OR_RETURN(NotHopelessDeficit, AddNode(NotGettingCrushed, &GameStateEvaluator::IsBlowoutMargin, GameStates::HopelessDeficit, false));
}

void GameStateEvaluator::AddPerformanceBranch()
{
	AddBranch("Performance", std::make_shared<DecisionNode<GameStates, bool, int>>(&GameStateEvaluator::IsWinning, GameStates::TippingPoint));
	DECL_GET_OR_RETURN(Performance, GetBranchNode("Performance"));

	auto PerformanceResetNode = std::make_shared<DecisionNode<GameStates, bool, int>>(
		[](int _) { return true; }, GameStates::TippingPoint);

	DECL_GET_OR_RETURN(IsAStalledLead, AddNode(Performance, std::bind(&GameStateEvaluator::IsAStalledLead), GameStates::StalledLead, true));
	DECL_GET_OR_RETURN(IsNeckAndNeck1, AddNode(IsAStalledLead, &GameStateEvaluator::IsNeckAndNeck, GameStates::NeckAndNeck, false));
	IsNeckAndNeck1->m_false = PerformanceResetNode;

	DECL_GET_OR_RETURN(IsLosingMomentum, AddNode(IsAStalledLead, std::bind(&GameStateEvaluator::IsLosingMomentum), GameStates::LosingMomentum, true));
	DECL_GET_OR_RETURN(IsUnderPressure, AddNode(IsLosingMomentum, std::bind(&GameStateEvaluator::IsUnderPressure), GameStates::UnderPressure, true));

	// A ComebackInProgress can transition into ClosingTheGap and eventually SmellingBlood if the trailing player gets close to tying the score.
	DECL_GET_OR_RETURN(IsAComebackInProgress, AddNode(Performance, std::bind(&GameStateEvaluator::IsComebackInProgress), GameStates::ComebackInProgress, false));
	DECL_GET_OR_RETURN(IsNeckAndNeck2, AddNode(IsAComebackInProgress, &GameStateEvaluator::IsNeckAndNeck, GameStates::NeckAndNeck, false));
	IsNeckAndNeck2->m_false, PerformanceResetNode;

	DECL_GET_OR_RETURN(IsClosingTheGap, AddNode(IsAComebackInProgress, std::bind(&GameStateEvaluator::IsClosingTheGap), GameStates::ClosingTheGap, true));
	DECL_GET_OR_RETURN(IsSmellingBlood, AddNode(IsClosingTheGap, std::bind(&GameStateEvaluator::IsSmellingBlood), GameStates::SmellingBlood, true));
}

void GameStateEvaluator::AddCriticalBranch()
{
	AddBranch("Critical", std::make_shared<DecisionNode<GameStates, bool, int>>(&GameStateEvaluator::IsWinning, GameStates::NonCritical));
	DECL_GET_OR_RETURN(Critical, GetBranchNode("Critical"));

	auto CriticalResetNode = std::make_shared<DecisionNode<GameStates, bool, int>>(
		[](int _) { return true; }, GameStates::NonCritical);

	DECL_GET_OR_RETURN(IsAtMatchPoint, AddNode(Critical, std::bind(GameStateEvaluator::IsAtMatchPoint), GameStates::MatchPoint, true));
	IsAtMatchPoint->m_false = CriticalResetNode;

	DECL_GET_OR_RETURN(IsInSuddenDeath1, AddNode(IsAtMatchPoint, std::bind(GameStateEvaluator::IsInSuddenDeath), GameStates::SuddenDeath, true));
	DECL_GET_OR_RETURN(HasFailedToCloseTheMatch, AddNode(IsInSuddenDeath1, std::bind(GameStateEvaluator::HasFailedToCloseTheGame), GameStates::FailedToCloseTheGame, false));

	DECL_GET_OR_RETURN(IsFacingElimination, AddNode(Critical, std::bind(GameStateEvaluator::IsFacingElimination), GameStates::FacingElimination, false));
	IsFacingElimination->m_false = CriticalResetNode;

	DECL_GET_OR_RETURN(IsInSuddenDeath2, AddNode(IsFacingElimination, std::bind(GameStateEvaluator::IsInSuddenDeath), GameStates::SuddenDeath, true));
	DECL_GET_OR_RETURN(HasSavedTheMatch, AddNode(IsInSuddenDeath2, std::bind(GameStateEvaluator::HasSavedTheGame), GameStates::GamePointSaved, false));
}

bool GameStateEvaluator::IsNeckAndNeck(int standing)
{
	if (std::abs(standing) < 1)
		return false;

	int counter = 0;
	PlayerIdentifiers id = m_goalHistory.back();

	for (auto it = m_goalHistory.rbegin() + 1; it != m_goalHistory.rend(); ++it)
	{
		if (*it == id)
			break;

		counter++;
		id = *it;
	}

	return counter > 2;
}

bool GameStateEvaluator::IsAStalledLead()
{
	return GetScoreStreak(false) > 2;
}

bool GameStateEvaluator::IsLosingMomentum()
{
	int streak = GetScoreStreak(false);

	if (streak < 3)
		return false;

	return HadDominantLeadBefore(true, streak);
}

bool GameStateEvaluator::IsUnderPressure()
{
	int streak = GetScoreStreak(false);

	if (streak < 2)
		return false;

	int myScore, opsScore;
	if (!HadDominantLeadBefore(true, streak, myScore, opsScore))
		return false;

	opsScore += streak;

	return myScore - opsScore <= 2;
}

bool GameStateEvaluator::IsComebackInProgress()
{
	return GetScoreStreak(true) > 0;
}

bool GameStateEvaluator::IsClosingTheGap()
{
	int streak = GetScoreStreak(true);

	if (streak < 3)
		return false;

	return HadDominantLeadBefore(false, streak);
}

bool GameStateEvaluator::IsSmellingBlood()
{
	int streak = GetScoreStreak(true);

	if (streak < 2)
		return false;

	int opsScore, myScore;
	if (!HadDominantLeadBefore(false, streak, opsScore, myScore))
		return false;

	opsScore += streak;

	return myScore - opsScore <= 2;
}

bool GameStateEvaluator::IsAtMatchPoint()
{
	return GetMyScore() == 8;
}

bool GameStateEvaluator::HasFailedToCloseTheGame()
{
	return GetScoreStreak(false) > 1;
}

bool GameStateEvaluator::IsFacingElimination()
{
	return GetOpponentScore() == 8;
}

bool GameStateEvaluator::HasSavedTheGame()
{
	return GetScoreStreak(true) > 1;
}

bool GameStateEvaluator::IsInSuddenDeath()
{
	return IsAtMatchPoint() && IsFacingElimination();
}

int GameStateEvaluator::GetScoreStreak(bool mine)
{
	auto predicate = mine
		? [](int scorer) { return scorer == m_currentAnalyser; }
	: [](int scorer) { return scorer != m_currentAnalyser; };

	return static_cast<int>(std::ranges::count_if(m_goalHistory | std::views::reverse, predicate));
}

int GameStateEvaluator::GetMyScore(int excludeLastN)
{
	return static_cast<int>(std::count(m_goalHistory.begin(), m_goalHistory.end() - excludeLastN, m_currentAnalyser));
}

int GameStateEvaluator::GetOpponentScore(int excludeLastN)
{
	return static_cast<int>(m_goalHistory.size() - excludeLastN) - GetMyScore(excludeLastN);
}

bool GameStateEvaluator::HadDominantLeadBefore(bool mine, int streak)
{
	int myScore, opponentScore;
	return HadDominantLeadBefore(mine, streak, myScore, opponentScore);
}

bool GameStateEvaluator::HadDominantLeadBefore(bool mine, int streak, int& scoreToBeChecked, int& otherScore)
{
	scoreToBeChecked = mine ? GetMyScore(streak) : GetOpponentScore(streak);
	otherScore = mine ? GetOpponentScore(streak) : GetMyScore(streak);

	return scoreToBeChecked >= otherScore + 3;
}