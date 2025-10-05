#include "MainMenuState.h"

#include "MainState.h"
#include "../Scenes/GameCourt.h"
#include "../Scenes/PracticeCourt.h"
#include "../../GameObjects/Player.h"
#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include <UI/SFMenu.h>
#include <Engine/Core/Constants.h>

enum MenuType
{
	Initial, PlayerSelect, TwoPlayerSelect,  DiffSelect
};

MainMenuState::MainMenuState(GameManager* gameMgr)
	: IGameState(gameMgr),
	m_gamenameMessage(TextConfig("Pong", 180, { GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 8.f }, TextAnimType::Static))
{}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture("Title");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Vector2f());

	m_gamenameMessage.SetText("PONG");

	Vector2f menuSize = { GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f };
	MenuPositionData posData = { MenuPositionMode::Centered, { GameConstants::ScreenDim.x / 2.0f, GameConstants::ScreenDim.y / 1.5f } };

	TextConfig textConfig;
	textConfig.m_fontName = "Pong";
	textConfig.m_animType = TextAnimType::Flashing;
	textConfig.m_alignment = TextAlignment::Center;
	textConfig.m_colour = Colour::Black;

	AddInitialMenu(menuSize, posData, textConfig);
	Add1PlayerSideMenu(menuSize, posData, textConfig);
	Add2PlayerSideMenu(menuSize, posData, textConfig);
	AddDifficultyMenu(menuSize, posData, textConfig);
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
	PerformMenuActions();
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_gamenameMessage.Update(deltaTime);
	m_menu.Update(deltaTime);
}

void MainMenuState::Render()
{
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(renderer, m_gameMgr->GetRenderer());

	m_backgroundSpr.Render(renderer);
	m_gamenameMessage.Render(renderer);
	m_menu.Render(renderer);
}

void MainMenuState::PerformMenuActions()
{
	ENSURE_VALID(m_gameMgr);
	GET_OR_RETURN(inputMgr, m_gameMgr->GetInputManager());

	if (inputMgr->GetKeyState((int)KeyCode::Enter))
	{
		if (m_canProceed)
		{
			switch (m_menu.GetCurrentMenuNumber())
			{
			case Initial:
				PerformInitialMenuActions();
				break;
			case PlayerSelect:
				Perform1PlayerSideMenuActions();
				break;
			case TwoPlayerSelect:
				Perform2PlayerSideMenuActions();
				break;
			case DiffSelect:
				PerformDifficultyMenuActions();
				break;
			}

			m_canProceed = false;
		}
	}
	else
	{
		m_canProceed = true;
	}
}

void MainMenuState::PerformInitialMenuActions()
{
	GET_OR_RETURN(menu, m_menu.GetCurrentMenu());
	GET_OR_RETURN(menuNav, menu->GetMenuNav());

	auto val = menuNav->GetCurrCursorPos();
	switch (menuNav->GetCurrCursorPos())
	{
	case VsAI:
		GameMode::s_type = VsAI;
		m_menu.SetCurrentMenuNumber(PlayerSelect);
		break;
	case vsWall:
		GameMode::s_type = vsWall;
		m_menu.SetCurrentMenuNumber(PlayerSelect);
		break;
	case VsPly:
		GameMode::s_type = VsPly;
		m_menu.SetCurrentMenuNumber(TwoPlayerSelect);
		break;
	case AIvsAI:
		GameMode::s_type = AIvsAI;
		GameMode::AssigPlayer1(Player1);
		GameMode::AssigPlayer2(Player2);
		m_menu.SetCurrentMenuNumber(DiffSelect);
		break;
	}
}

void MainMenuState::ChooseSide(PlayerIdentifiers id)
{
	GET_OR_RETURN(menu, m_menu.GetCurrentMenu());
	GET_OR_RETURN(cursor, menu->GetCursor(id));
	GET_OR_RETURN(menuNav, cursor->GetMenuNav());

	auto val = menuNav->GetCurrCursorPos();
	switch (menuNav->GetCurrCursorPos())
	{
	case 0:
	case 3:
		switch (id)
		{
		case Player1:
			GameMode::AssigPlayer1(Player1);
			break;
		case Player2:
			GameMode::AssigPlayer2(Player1);
			break;
		}
		break;
	case 1:
	case 4:
		switch (id)
		{
		case Player1:
			GameMode::AssigPlayer1(Unset);
			break;
		case Player2:
			GameMode::AssigPlayer2(Unset);
			break;
		}
		break;
	case 2:
	case 5:
		switch (id)
		{
		case Player1:
			GameMode::AssigPlayer1(Player2);
			break;
		case Player2:
			GameMode::AssigPlayer2(Player2);
			break;
		}
		break;
	}
}

void MainMenuState::Perform1PlayerSideMenuActions()
{
	ChooseSide(Player1);

	if (GameMode::ArePlayersAssigned())
	{
		m_menu.SetCurrentMenuNumber(DiffSelect);
	}
}

void MainMenuState::Perform2PlayerSideMenuActions()
{
	ChooseSide(Player1);
	ChooseSide(Player2);

	if (GameMode::ArePlayersAssigned())
	{
		m_menu.SetCurrentMenuNumber(DiffSelect);
	}
}

void MainMenuState::PerformDifficultyMenuActions()
{
	GET_OR_RETURN(menu, m_menu.GetCurrentMenu());
	GET_OR_RETURN(menuNav, menu->GetMenuNav());

	bool ready = false;

	auto menu = m_menu.GetCurrentMenu();

	switch (menuNav->GetCurrCursorPos())
	{
	case Easy:
		DifficultyMode::EasyMode();
		ready = true;
		break;
	case Normal:
		DifficultyMode::MediumMode();
		ready = true;
		break;
	case Hard:
		DifficultyMode::HardMode();
		ready = true;
		break;
	}

	if (ready)
	{
		GET_OR_RETURN(gameMgr, GameManager::Get());

		if (GameMode::s_type != vsWall)
			gameMgr->SetScene(std::make_shared<GameCourt>());
		else
			gameMgr->SetScene(std::make_shared<PracticeCourt>());

		gameMgr->GetGameStateMgr()->ChangeState(new MainState(m_gameMgr));
	}
}

void MainMenuState::AddInitialMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config)
{
	GET_OR_RETURN(menu, m_menu.AddMenu(new SFMenu(menuSize, 2.f, { 1,4 }, posData)));

	auto cellSize = menu->GetCellSize();

	GET_OR_RETURN(cell, menu->GetCell({ 0, 0 }));
	config.m_charSize = (int)(cellSize.y * 0.6f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Player vs. A.I.");
	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 0 }));
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Player vs. Player");
	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, menu->GetCell({ 2, 0 }));
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "A.I vs. A.I");
	cell->SetMenuSlotNumber(2);

	GET_OR_RETURN(cell, menu->GetCell({ 3, 0 }));
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Player vs. Wall");
	cell->SetMenuSlotNumber(3);

	menu->SetActiveCells();

	GET_OR_RETURN(menuNav, menu->GetMenuNav());

	menuNav->SetCursorRange({ 0,1,2,3 });
	menuNav->SetCurrCursorPos(0);
}

void MainMenuState::Add1PlayerSideMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config)
{
	GET_OR_RETURN(menu, m_menu.AddMenu(new SFMenu(menuSize, 2.f, { 3,2 }, posData)));
	auto menu = m_menu.AddMenu(new SFMenu(menuSize, 2.f, { 3,2 }, posData));

	auto cellSize = menu->GetCellSize();

	GET_OR_RETURN(cell, menu->GetCell({ 0, 0 }));

	config.m_animType = TextAnimType::Static;
	config.m_charSize = (int)(cellSize.y * 0.25f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFText>(config)));
	text->SetText("Player 1");

	GET_OR_RETURN(cell, menu->GetCell({ 0, 2 }));
	config.m_charSize = (int)(cellSize.y * 0.25f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFText>(config)));
	text->SetText("Player 2");

	GET_OR_RETURN(cell, menu->GetCell({ 1, 0 }));
	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 1 }));
	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 2 }));
	cell->SetMenuSlotNumber(2);

	menu->SetActiveCells();

	menu->AddCursor(new SFSprite("CtrlCursor"), MenuNav(KeyCode::Left, KeyCode::Right));
	GET_OR_RETURN(cursor, menu->GetCursor(0));
	cursor->SetScale(cellSize);

	GET_OR_RETURN(menuNav, cursor->GetMenuNav());

	menuNav->SetCursorRange({ 0,1,2 });
	menuNav->SetCurrCursorPos(1);
}

void MainMenuState::Add2PlayerSideMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config)
{
	GET_OR_RETURN(menu, m_menu.AddMenu(new SFMenu(menuSize, 2.f, { 3,3 }, posData)));

	auto cellSize = menu->GetCellSize();

	GET_OR_RETURN(cell, menu->GetCell({ 0, 0 }));

	config.m_animType = TextAnimType::Static;
	config.m_charSize = (int)(cellSize.y * 0.375f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFText>(config)));
	text->SetText("Player 1");

	GET_OR_RETURN(cell, menu->GetCell({ 0, 2 }));
	config.m_charSize = (int)(cellSize.y * 0.375f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFText>(config)));
	text->SetText("Player 2");

	GET_OR_RETURN(cell, menu->GetCell({ 1, 0 }));
	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 1 }));
	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 2 }));
	cell->SetMenuSlotNumber(2);

	GET_OR_RETURN(cell, menu->GetCell({ 2, 0 }));
	cell->SetMenuSlotNumber(3);

	GET_OR_RETURN(cell, menu->GetCell({ 2, 1 }));
	cell->SetMenuSlotNumber(4);

	GET_OR_RETURN(cell, menu->GetCell({ 2, 2 }));
	cell->SetMenuSlotNumber(5);

	menu->SetActiveCells();

	menu->AddCursor(new SFSprite("CtrlCursor"), MenuNav(KeyCode::A, KeyCode::D));
	GET_OR_RETURN(cursor, menu->GetCursor(0));

	cursor->SetScale(cellSize);

	GET_OR_RETURN(menuNav, cursor->GetMenuNav());

	menuNav->SetCursorRange({ 0,1,2 });
	menuNav->SetCurrCursorPos(1);

	menu->AddCursor(new SFSprite("CtrlCursor"), MenuNav(KeyCode::J, KeyCode::L));
	GET_OR_RETURN(cursor, menu->GetCursor(1));

	cursor->SetScale(cellSize);

	GET_OR_RETURN(menuNav, cursor->GetMenuNav());

	menuNav->SetCursorRange({ 3,4,5 });
	menuNav->SetCurrCursorPos(4);
}

void MainMenuState::AddDifficultyMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config)
{
	GET_OR_RETURN(menu, m_menu.AddMenu(new SFMenu(menuSize, 2.f, { 3,2 }, posData)));

	auto cellSize = menu->GetCellSize();

	GET_OR_RETURN(cell, menu->GetCell({ 1, 0 }));

	config.m_animType = TextAnimType::Flashing;
	config.m_charSize = (int)(cellSize.y * 0.25f);
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), ("Easy"));

	cell->SetMenuSlotNumber(0);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 1 }));
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Medium");

	cell->SetMenuSlotNumber(1);

	GET_OR_RETURN(cell, menu->GetCell({ 1, 2 }));
	config.m_position = cell->GetPosition();

	GET_OR_RETURN(text, cell->AddTextElement(std::make_shared<SFAnimatedText>(config)));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Hard");

	cell->SetMenuSlotNumber(2);

	menu->SetActiveCells();

	GET_OR_RETURN(menuNav, menu->GetMenuNav());

	menuNav->ChangeNavKeys(KeyCode::Left, KeyCode::Right);
	menuNav->SetCursorRange({ 0, 1, 2 });
	menuNav->SetCurrCursorPos(1);
}