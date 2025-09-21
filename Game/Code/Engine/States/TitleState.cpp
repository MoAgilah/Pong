#include "TitleState.h"

#include "MainMenuState.h"
#include <Engine/Core/Constants.h>

TitleState::TitleState(GameManager* gameMgr)
	: IGameState(gameMgr),
	m_gamenameMessage({ "Pong",  180, { GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 8.f }, TextAnimType::Static }),
	m_titleMessage({ "Pong",  45, GameConstants::ScreenDim / 2.0f, TextAnimType::Flashing })
{}

void TitleState::Initialise()
{
	m_backgroundSpr.SetTexture("Title");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	m_gamenameMessage.SetText("PONG");

	InitFlashingText(&m_titleMessage, "Press Any Key To Start", true);
}

void TitleState::Pause()
{
}

void TitleState::Resume()
{
}

void TitleState::ProcessInputs()
{
	if (m_gameMgr->GetInputManager()->IsAnyKeyPressed())
		m_gameMgr->GetGameStateMgr()->ChangeState(new MainMenuState(m_gameMgr));
}

void TitleState::Update(float deltaTime)
{
	ProcessInputs();

	m_titleMessage.Update(deltaTime);
}

void TitleState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_backgroundSpr.Render(renderer);
	m_gamenameMessage.Render(renderer);
	m_titleMessage.Render(renderer);
}