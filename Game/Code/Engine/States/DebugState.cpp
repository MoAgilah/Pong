#include "DebugState.h"

#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <iostream>

DebugState::DebugState(GameManager* gameMgr)
	: IGameState(gameMgr), m_countdownConfig("Pong", 150, GameConstants::ScreenDim / 2.0f, TextAnimType::Countdown),
	m_sfText(m_countdownConfig)
{}

void DebugState::Initialise()
{
	m_sfText.InitCountdownText(3, "Go!");
}

void DebugState::Pause()
{
}

void DebugState::Resume()
{
}

void DebugState::ProcessInputs()
{
}

void DebugState::Update(float deltaTime)
{
	m_sfText.Update(deltaTime);
}



void DebugState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_sfText.Render(renderer);
}
