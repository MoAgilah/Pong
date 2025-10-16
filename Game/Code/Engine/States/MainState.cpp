#include "MainState.h"

#include "LoadingState.h"
#include "PauseMenuState.h"
#include <Engine/Core/Constants.h>
#include <Engine/Interface/Collisions/IBoundingVolume.h>
#include <Utilities/Utils.h>

MainState::MainState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void MainState::Initialise()
{
	ENSURE_VALID(m_gameMgr);
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
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState((int)KeyCode::Enter))
		m_gameMgr->GetGameStateMgr()->PushState(new PauseMenuState(m_gameMgr));
}

void MainState::Update(float deltaTime)
{
	ENSURE_VALID(m_gameMgr);
	m_gameMgr->GetTimer().Update(deltaTime);
	m_gameMgr->CheckInView();

	DECL_GET_OR_RETURN(scene, m_gameMgr->GetScene());
	scene->Update(deltaTime);
}

void MainState::Render()
{
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_gameMgr->GetCamera()->Reset(renderer);
	m_gameMgr->GetScene()->Render(renderer);

	if (GameConstants::DRender)
	{
		m_gameMgr->GetCollisionMgr()->Render(renderer);
		m_gameMgr->GetCamera()->RenderDebug(renderer);
	}
}