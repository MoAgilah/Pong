#pragma once

#include <Drawables/SFText.h>
#include <Drawables/SFSprite.h>
#include <UI/SFPaginatedMenu.h>
#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>

enum PlayerIdentifiers;

class MainMenuState : public IGameState
{
public:
	explicit MainMenuState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "MainMenu"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	void PerformMenuActions();
	void PerformInitialMenuActions();

	void ChooseSide(PlayerIdentifiers id);

	void Perform1PlayerSideMenuActions();
	void Perform2PlayerSideMenuActions();
	void PerformDifficultyMenuActions();

	void AddInitialMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config);
	void Add1PlayerSideMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config);
	void Add2PlayerSideMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config);
	void AddDifficultyMenu(const Vector2f& menuSize, const MenuPositionData& posData, TextConfig& config);

	bool m_canProceed = false;
	SFSprite m_backgroundSpr;
	SFText m_gamenameMessage;
	SFPaginatedMenu m_menu;
};