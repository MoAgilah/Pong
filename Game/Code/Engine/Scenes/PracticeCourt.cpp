#include "PracticeCourt.h"

#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Player.h"
#include "../../GameObjects/Wall.h"
#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include "../../Engine/Core/PongFramework.h"
#include <Drawables/SFSprite.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

PracticeCourt::PracticeCourt()
{
	m_backgroundSpr = std::make_shared<SFSprite>("Court");
	DECL_GET_OR_RETURN(bkgSpr, dynamic_cast<SFSprite*>(m_backgroundSpr.get()));

	bkgSpr->SetScale(GameConstants::Scale);
	bkgSpr->SetOrigin(GameConstants::ScreenDim / 2.f);
	bkgSpr->SetPosition(GameConstants::ScreenDim / 2.f);
}

void PracticeCourt::Update(float deltaTime)
{
	UpdateGUI(deltaTime);

	DECL_GET_OR_RETURN(ball, dynamic_cast<Ball*>(GetObjectByName("Ball")));

	if (m_matchCtrl.IsReady())
	{
		Player* ply = nullptr;

		for (auto& [name, object] : m_objects)
		{
			CONTINUE_IF_INVALID(object);

			if (!object->GetActive())
				continue;

			object->Update(deltaTime);
		}

		if (GameMode::s_type == vsWall)
			m_matchCtrl.SetPlayerScore(Player1, ball->GetRallieCount());

		DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
		DECL_GET_OR_RETURN(camera, gameMgr->GetCamera());

		if (!camera->IsInView(ball->GetVolume()))
		{
			ball->SetLastRallieCount();

			if (GameMode::s_type != vsWall)
			{
				if (ball->GetPosition().x < 0)
				{
					m_matchCtrl.IncrementPlayerScore(Player2);
				}
				else if (ball->GetPosition().x > GameConstants::ScreenDim.x)
				{
					m_matchCtrl.IncrementPlayerScore(Player1);
				}
			}
			else
			{
				m_matchCtrl.SetPlayerScore(Player2, ball->GetHighestRallieCount());
			}

			for (auto& [_, object] : m_objects)
			{
				CONTINUE_IF_INVALID(object);

				object->Reset();
			}
		}
	}
}

void PracticeCourt::ResetScene()
{
	m_matchCtrl.Reset();
	IScene::ResetScene();
}

void PracticeCourt::AddEnemies()
{
}

void PracticeCourt::AddObjects()
{
	m_objects.emplace("Ball", std::make_shared<Ball>(Vector2f(GameConstants::ScreenDim / 2.0f)));

	m_objects.emplace("FirstPlayer", std::make_shared<Player>(GameMode::s_player1));

	switch (GameMode::s_player1)
	{
	case Player1:
		m_objects.emplace("SideWall", std::make_shared<Wall>(SetVerticalWall(Vector2f(GameConstants::ScreenDim.y - (DifficultyMode::wallThickness / 2.f), GameConstants::ScreenDim.x / 2.f))));
		break;
	case Player2:
		m_objects.emplace("SideWall", std::make_shared<Wall>(SetVerticalWall(Vector2f(DifficultyMode::wallThickness / 2.f, GameConstants::ScreenDim.x / 2.f))));
		break;
	}

	m_ballPhysics.SetCurrentBall(static_cast<Ball*>(GetObjectByName("Ball")));
}

void PracticeCourt::AddForeGroundObjects()
{
	m_objects.emplace("TopWall", std::make_shared<Wall>(SetHorizontalWall(Vector2f(GameConstants::ScreenDim.x / 2.f, DifficultyMode::wallThickness * 2.f))));
	m_objects.emplace("BottomWall", std::make_shared<Wall>(SetHorizontalWall(Vector2f(GameConstants::ScreenDim.x / 2.f, GameConstants::ScreenDim.y - (DifficultyMode::wallThickness / 2.f)))));
}

void PracticeCourt::SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos)
{
}

void PracticeCourt::AddGUI()
{
	m_matchCtrl.SetRallyTallying();

	float charSize = GameConstants::ScreenDim.y * 0.04f;

	m_matchCtrl.PlacePlayersScoreCard(Player1, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.167f, charSize * 0.75f));
	m_matchCtrl.PlacePlayersScoreCard(Player2, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.833f, charSize * 0.75f));
}

void PracticeCourt::UpdateGUI(float deltaTime)
{
	m_matchCtrl.Update(deltaTime);
}

void PracticeCourt::RenderGUI(IRenderer* renderer)
{
	ENSURE_VALID(renderer);

	m_matchCtrl.Render(renderer);
}