#include "MatchCtrl.h"

#include "GameMode.h"
#include "../GameObjects/Player.h"
#include <Engine/Core/Constants.h>
#include <format>

int MatchCtrl::m_currentResults = NeckAndNeck;

MatchCtrl::MatchCtrl()
	: m_scoreBoard(GameConstants::ScreenDim / 2.f),
	m_countdownConfig("Pong", 150, GameConstants::ScreenDim / 2.0f, TextAnimType::Countdown),
	m_resultsConfig("Pong", 70, GameConstants::ScreenDim / 2.0f, TextAnimType::Flashing, Colour::Black, TextAlignment::Center),
	m_centerText(m_countdownConfig)
{
	m_centerText.InitCountdownText(3, "Go!");
}

void MatchCtrl::SetRallyTallying()
{
	m_rallyTalling = true;
	m_scoreBoard.SetRallyTally();
}

void MatchCtrl::Update(float deltaTime)
{
	if (!m_rallyTalling)
	{
		std::string WinningPlayer = "";
		for (int i = 0; i < PlayerIdentifiers::MaxPlayers; i++)
		{
			if (m_scoreBoard.GetPlayerScore((PlayerIdentifiers)i) == m_winningScore)
			{
				m_gameOver = true;
				switch (i)
				{
				case PlayerIdentifiers::Player1:
					WinningPlayer = "PLAYER 1";
					break;
				case PlayerIdentifiers::Player2:
					WinningPlayer = "PLAYER 2";
					break;
				}
			}
		}

		if (m_gameOver)
		{
			m_centerText = SFAnimatedText(m_resultsConfig);
			m_centerText.SetText(std::format("{} WINS!", WinningPlayer));
			m_centerText.SetIsPaused(false);
		}
	}

	m_centerText.Update(deltaTime);
	m_scoreBoard.Update(deltaTime);
}

void MatchCtrl::Render(IRenderer* renderer)
{
	m_centerText.Render(renderer);
	m_scoreBoard.Render(renderer);
}

void MatchCtrl::Reset()
{
	m_scoreBoard.Reset();
	m_centerText = SFAnimatedText(m_countdownConfig);
	m_centerText.InitCountdownText(3, "Go!");
}

void MatchCtrl::PlacePlayersScoreCard(PlayerIdentifiers playerID, int charSize, const Vector2f& pos)
{
	switch (playerID)
	{
	case Player1:
		m_scoreBoard.Place(playerID, charSize, Vector2f(pos.x, pos.y));
		break;
	case Player2:
		m_scoreBoard.Place(playerID, charSize, Vector2f(pos.x, pos.y));
		break;
	}
}

void MatchCtrl::SetCurrentMatchResults(PlayerIdentifiers id)
{
	int myScore = m_scoreBoard.GetPlayerScore(id);
	int opScore = m_scoreBoard.GetPlayerScore((id == Player1) ? Player2 : Player1);

	int standing = myScore - opScore;

	m_evaluator.SetCurrentAnalyser(id);

	m_evaluator.Evaluate(standing);
	for (const auto& res : m_evaluator.GetResults())
		m_currentResults |= res;
}

int MatchCtrl::GetCurrentMatchResults()
{
	return m_currentResults;
}

void MatchCtrl::SetPlayerScore(PlayerIdentifiers playerID, int val)
{
	m_scoreBoard.SetPlayerScore(playerID, val);
}

void MatchCtrl::IncrementPlayerScore(PlayerIdentifiers id)
{
	m_scoreBoard.IncrementPlayerScore(id);
	GameStateEvaluator::AddNewGoalHistoryEntry(id);
}

int MatchCtrl::GetPlayerScore(PlayerIdentifiers id)
{
	return m_scoreBoard.GetPlayerScore(id);
}