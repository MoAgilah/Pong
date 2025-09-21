#pragma once

#include <Drawables/SFText.h>
#include <Engine/Interface/Renderer/IRenderer.h>
#include <Utilities/Vector.h>
#include <array>
#include <memory>
#include <string>

enum PlayerIdentifiers;

class ScoreBoard
{
public:
	ScoreBoard(const Vector2f& center);
	~ScoreBoard() = default;

	void SetRallyTally() { m_rallyTallying = true; }

	void Place(PlayerIdentifiers playerID, int charSize, const Vector2f& pos);

	void SetPlayerScore(PlayerIdentifiers playerID, int val);
	void IncrementPlayerScore(PlayerIdentifiers playerID);
	int GetPlayerScore(PlayerIdentifiers playerID);

	void Reset();

	void Update(float deltaTime);
	void Render(IRenderer* renderer);

private:

	bool m_rallyTallying = false;
	std::array<int, 2> m_playersScore;
	std::array<std::unique_ptr<SFText>, 2> m_playersScoreText;
};