#pragma once

#include "../../Utilities/BallPhysics.h"
#include "../../Utilities/MatchCtrl.h"
#include <Engine/Interface/Collisions/IBoundingVolume.h>
#include <Engine/Interface/Scene/IScene.h>
#include <array>

class PracticeCourt : public IScene
{
public:
	PracticeCourt();
	~PracticeCourt() = default;

	void Update(float deltaTime) final;
	void ResetScene() final;

	void AddGUI() final;
	void AddEnemies() final;
	void AddObjects() final;
	void AddForeGroundSprites() final;

private:

	void UpdateGUI(float deltaTime) final;
	void RenderGUI(IRenderer* renderer) final;

	MatchCtrl m_matchCtrl;
	BallPhysics m_ballPhysics;
};