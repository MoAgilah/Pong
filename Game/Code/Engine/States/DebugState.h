#pragma once

#include <Drawables/SFText.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>
#include <memory.h>

class DebugState final : public IGameState
{
public:
	explicit DebugState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Debug"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	TextConfig m_countdownConfig;
	SFAnimatedText m_sfText;
};