#include "PauseMenuState.h"

#include "TitleState.h"
#include <Engine/Core/Constants.h>
#include <Utilities/Utils.h>

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

	DECL_GET_OR_RETURN(cell,m_menu.GetCell({ 0, 0 }));
	textConfig.m_charSize = (int)(cellSize.y * 0.4f);
	textConfig.m_position = cell->GetPosition();

	DECL_GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig)));
	DECL_GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Resume Current Game");

	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, m_menu.GetCell({ 1, 0 }));
	textConfig.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig)));
	GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Restart Current Game");

	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, m_menu.GetCell({ 2, 0 }));
	textConfig.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(textConfig)));
	GET_OR_RETURN(sfText, dynamic_cast<SFAnimatedText*>(text));
	sfText->InitFlashingText("Quit To Main Menu");

	cell->SetMenuSlotNumber(2);

	m_menu.SetActiveCells();

	DECL_GET_OR_RETURN(menuNav, m_menu.GetMenuNav());

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
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState((int)KeyCode::Enter))
		PerformMenuActions();
}

void PauseMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void PauseMenuState::Render()
{
	ENSURE_VALID(m_gameMgr);
	DECL_GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);

	m_menu.Render(renderer);
}

void PauseMenuState::PerformMenuActions()
{
	ENSURE_VALID(m_gameMgr);
	auto gameStateMgr = m_gameMgr->GetGameStateMgr();

	DECL_GET_OR_RETURN(menuNav, m_menu.GetMenuNav());

	switch (menuNav->GetCurrCursorPos())
	{
	case Continue:
		gameStateMgr->PopState();
		break;
	case Restart:
	{
		DECL_GET_OR_RETURN(scene, m_gameMgr->GetScene());
		scene->ResetScene();
		gameStateMgr->PopState();
	}
		break;
	case ToMain:
		gameStateMgr->ChangeState(new TitleState(m_gameMgr));
		break;
	}
}
