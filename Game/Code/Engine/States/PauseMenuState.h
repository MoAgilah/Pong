#pragma once

#include <Drawables/SFText.h>
#include <UI/SFMenu.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

class PauseMenuState : public IGameState
{
public:
	explicit PauseMenuState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "PauseMenu"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	void PerformMenuActions();

	SFMenu m_menu;
	SFSprite m_backgroundSpr;
	GameManager* m_gameMgr;
};