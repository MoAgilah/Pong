#include "ScoreBoard.h"

#include "../../GameObjects/Player.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/GameManager.h>
#include <format>

ScoreBoard::ScoreBoard(const Vector2f& center)
{
	Reset();

	TextConfig config;
	config.m_fontName = "Pong";

	for (auto& text : m_playersScoreText)
		text = std::make_unique<SFText>(config);

	m_playersScore.fill(0);
}

void ScoreBoard::Place(PlayerIdentifiers playerID, int charSize, const Vector2f& pos)
{
	auto& currentScore = m_playersScoreText[playerID];

	TextConfig config;
	config.m_position = pos;
	config.m_charSize = charSize;
	config.m_colour = Colour::Black;
	config.m_alignment = TextAlignment::Center;

	currentScore->SetOutlineThickness(2);

	if (m_rallyTallying)
	{
		switch (playerID)
		{
		case Player1:
			currentScore->Reset(std::format("Current Rallie Count: {}", m_playersScore[playerID]), config);
			break;
		case Player2:
			currentScore->Reset(std::format("Highest Rallie Count: {}", m_playersScore[playerID]), config);
			break;
		}
	}
	else
	{
		currentScore->Reset(std::to_string(m_playersScore[playerID]), config);
	}
}

void ScoreBoard::SetPlayerScore(PlayerIdentifiers playerID, int val)
{
	m_playersScore[playerID] = val;
}

void ScoreBoard::IncrementPlayerScore(PlayerIdentifiers playerID)
{
	m_playersScore[playerID]++;
}

int ScoreBoard::GetPlayerScore(PlayerIdentifiers playerID)
{
	return m_playersScore[playerID];
}

void ScoreBoard::Reset()
{
	m_playersScore.fill(0);
}

void ScoreBoard::Update(float deltaTime)
{
	if (m_rallyTallying)
	{
		m_playersScoreText[Player1]->SetText(std::format("Current Rallie Count: {}", m_playersScore[Player1]));
		m_playersScoreText[Player2]->SetText(std::format("Highest Rallie Count: {}", m_playersScore[Player2]));
	}
	else
	{
		for (size_t i = 0; i < PlayerIdentifiers::MaxPlayers; i++)
			m_playersScoreText[i]->SetText(std::to_string(m_playersScore[i]));
	}
}

void ScoreBoard::Render(IRenderer* renderer)
{
	for (auto& psText : m_playersScoreText)
		psText->Render(renderer);
}