#pragma once

#include <Engine/Core/GameManager.h>
#include <Engine/Interface/Scene/IGameState.h>
#include <memory>

class MainState final : public IGameState
{
public:
	explicit MainState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Main"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;
};