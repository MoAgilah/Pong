#include "MainState.h"

#include "LoadingState.h"
#include "PauseMenuState.h"
#include <Engine/Core/Constants.h>
#include <Engine/Interface/Collisions/IBoundingVolume.h>

MainState::MainState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void MainState::Initialise()
{
	m_gameMgr->GetGameStateMgr()->PushState(new LoadingState(m_gameMgr));
}

void MainState::Pause()
{
}

void MainState::Resume()
{
}

void MainState::ProcessInputs()
{
	if (m_gameMgr->GetInputManager()->GetKeyState((int)KeyCode::Enter))
		m_gameMgr->GetGameStateMgr()->PushState(new PauseMenuState(m_gameMgr));
}

void MainState::Update(float deltaTime)
{
	m_gameMgr->GetTimer().Update(deltaTime);
	m_gameMgr->CheckInView();
	m_gameMgr->GetScene()->Update(deltaTime);
}

void MainState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_gameMgr->GetCamera()->Reset(renderer);
	m_gameMgr->GetScene()->Render(renderer);
	m_gameMgr->GetCollisionMgr()->Render(renderer);
	m_gameMgr->GetCamera()->RenderDebug(renderer);
}