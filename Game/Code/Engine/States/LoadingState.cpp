#include "LoadingState.h"

#include "MainMenuState.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>
#include <thread>

void LoadResources()
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(scene, gameMgr->GetScene());

	scene->AddObjects();
	scene->AddEnemies();
	scene->AddForeGroundObjects();
	scene->AddGUI();

	std::this_thread::sleep_for(std::chrono::seconds(3));

	GameConstants::GameIsReady = true;
}

LoadingState::LoadingState(GameManager* gameMgr)
	: IGameState(gameMgr), m_gamenameMessage({ "Pong",  180, GameConstants::ScaleScreenDim(0.5f, 0.125f) , TextAnimType::Static }),
	m_loadingMessage({ "Pong", 60, { GameConstants::ScreenDim.x / 2.0f, (GameConstants::ScreenDim.y / 2.0f) + 30.f }, TextAnimType::Flashing })
{}

void LoadingState::Initialise()
{
	m_backgroundSpr.SetTexture("Title");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	m_gamenameMessage.SetText("PONG");
	m_loadingMessage.SetText("Loading");

	std::thread t(LoadResources);
	t.detach();
}

void LoadingState::Pause()
{
}

void LoadingState::Resume()
{
}

void LoadingState::ProcessInputs()
{
}

void LoadingState::Update(float deltaTime)
{
	ENSURE_VALID(m_gameMgr);

	ProcessInputs();

	m_loadingMessage.Update(deltaTime);

	if (GameConstants::GameIsReady)
		m_gameMgr->GetGameStateMgr()->PopState();
}

void LoadingState::Render()
{
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_gamenameMessage.Render(renderer);
	m_loadingMessage.Render(renderer);
}