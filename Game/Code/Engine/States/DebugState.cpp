#include "DebugState.h"

#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include <Engine/Core/Constants.h>
#include <iostream>

DebugState::DebugState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void DebugState::Initialise()
{
	m_ply = std::make_shared<Ball>(GameConstants::ScreenDim*0.5f);
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
    m_ply->Update(deltaTime);
}



void DebugState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_ply->Render(renderer);
}
