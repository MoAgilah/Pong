#pragma once

#include "../Engine/AI/GameStateEvaluator.h"
#include "../Engine/UI/ScoreBoard.h"
#include <Drawables/SFText.h>
#include <Engine/Interface/Renderer/IRenderer.h>
#include <Utilities/Vector.h>
#include <vector>

class MatchCtrl
{
public:
	MatchCtrl();
	~MatchCtrl() = default;

	void SetRallyTallying();

	void Update(float deltaTime);
	void Render(IRenderer* renderer);

	void Reset();

	bool IsReady() { return m_centerText.CountHasEnded(); }
	bool IsOver() { return m_gameOver; }

	void PlacePlayersScoreCard(PlayerIdentifiers playerID, int charSize, const Vector2f& pos);

	void SetCurrentMatchResults(PlayerIdentifiers id);
	static int GetCurrentMatchResults();

	void SetPlayerScore(PlayerIdentifiers playerID, int val);

	void IncrementPlayerScore(PlayerIdentifiers id);
	int GetPlayerScore(PlayerIdentifiers id);

private:

	static int m_currentResults;

	// Game State Variables
	int m_winningScore = 9;
	bool m_gameOver = false;
	bool m_rallyTalling = false;

	// UI Elements & Game Objects
	TextConfig m_countdownConfig;
	TextConfig m_resultsConfig;
	SFAnimatedText m_centerText;
	ScoreBoard m_scoreBoard;
	GameStateEvaluator m_evaluator;
};