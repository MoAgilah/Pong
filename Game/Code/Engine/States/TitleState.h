#pragma once

#include <Drawables/SFSprite.h>
#include <Drawables/SFText.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>
#include <memory>

class TitleState final : public IGameState
{
public:
	explicit TitleState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Title"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	SFSprite m_backgroundSpr;
	SFText m_gamenameMessage;
	SFAnimatedText m_titleMessage;
};