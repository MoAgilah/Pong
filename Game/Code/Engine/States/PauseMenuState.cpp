#include "PauseMenuState.h"

#include "TitleState.h"
#include <Engine/Core/Constants.h>

enum MenuPosition { Continue, Restart, ToMain };

PauseMenuState::PauseMenuState(GameManager* gameMgr)
	: IGameState(gameMgr),
	m_menu({ GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f }, 2.f, { 1,3 }, { MenuPositionMode::Centered, { GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 1.5f } })
{}

void PauseMenuState::Initialise()
{
	m_backgroundSpr.SetTexture("Title");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	// Set up menu
	TextConfig textConfig;
	textConfig.m_fontName = "Pong";
	textConfig.m_animType = TextAnimType::Flashing;
	textConfig.m_alignment = TextAlignment::Center;
	textConfig.m_colour = Colour::Black;

	auto cellSize = m_menu.GetCellSize();

	auto cell = m_menu.GetCell({ 0, 0 });
	textConfig.m_charSize = (int)(cellSize.y * 0.4f);
	textConfig.m_position = cell->GetPosition();

	auto text = cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text),"Resume Current Game");
	cell->SetMenuSlotNumber(0);

	cell = m_menu.GetCell({ 1, 0 });
	textConfig.m_position = cell->GetPosition();

	text = cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Restart Current Game");
	cell->SetMenuSlotNumber(1);

	cell = m_menu.GetCell({ 2, 0 });
	textConfig.m_position = cell->GetPosition();

	text = cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Quit To Main Menu");
	cell->SetMenuSlotNumber(2);

	m_menu.SetActiveCells();

	auto menuNav = m_menu.GetMenuNav();

	menuNav->SetCursorRange({ 0,1,2 });
	menuNav->SetCurrCursorPos(0);
}

void PauseMenuState::Pause()
{
}

void PauseMenuState::Resume()
{
}

void PauseMenuState::ProcessInputs()
{
	if (m_gameMgr->GetInputManager()->GetKeyState((int)KeyCode::Enter))
		PerformMenuActions();
}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void PauseMenuState::Render()
{
	auto renderer = m_gameMgr->GetRenderer();

	m_backgroundSpr.Render(renderer);

	m_menu.Render(renderer);
}

void PauseMenuState::PerformMenuActions()
{
	auto gameStateMgr = m_gameMgr->GetGameStateMgr();

	switch (m_menu.GetMenuNav()->GetCurrCursorPos())
	{
	case Continue:
		gameStateMgr->PopState();
		break;
	case Restart:
		m_gameMgr->GetScene()->ResetScene();
		gameStateMgr->PopState();
		break;
	case ToMain:
		gameStateMgr->ChangeState(new TitleState(m_gameMgr));
		break;
	}
}
