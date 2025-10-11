#include "DebugState.h"

#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <iostream>

DebugState::DebugState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void DebugState::Initialise()
{
	m_ball = std::make_shared<Ball>(GameConstants::ScreenDim * 0.5f);
	ENSURE_VALID(m_ball);
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
	ENSURE_VALID(m_ball);
    m_ball->Update(deltaTime);
}



void DebugState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_ball->Render(renderer);
}
