#include "GameCourt.h"

#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Player.h"
#include "../../GameObjects/Wall.h"
#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include "../../Engine/Core/PongFramework.h"
#include <Drawables/SFSprite.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

GameCourt::GameCourt()
{
	m_backgroundSpr = std::make_shared<SFSprite>("Court");
	DECL_GET_OR_RETURN(bkgSpr, dynamic_cast<SFSprite*>(m_backgroundSpr.get()));

	bkgSpr->SetScale(GameConstants::Scale);
	bkgSpr->SetOrigin(GameConstants::ScreenDim / 2.f);
	bkgSpr->SetPosition(GameConstants::ScreenDim / 2.f);
}

void GameCourt::Update(float deltaTime)
{
	UpdateGUI(deltaTime);

	DECL_GET_OR_RETURN(ball, dynamic_cast<Ball*>(GetObjectByName("Ball")));

	if (m_matchCtrl.IsReady() && !m_matchCtrl.IsOver())
	{
		Player* ply = nullptr;

		for (auto& [name, object] : m_objects)
		{
			CONTINUE_IF_INVALID(object);

			if (!object->GetActive())
				continue;

			if (name.ends_with("Player"))
			{
				GET_OR_RETURN(ply, dynamic_cast<Player*>(object.get()));

				m_matchCtrl.SetCurrentMatchResults(ply->GetPlayerID());

				GET_OR_CONTINUE(aiPly, dynamic_cast<AutomatedPlayer*>(ply));

				aiPly->UpdateFatigue(ball->GetRallieCount());
				auto id = aiPly->GetPlayerID();
				aiPly->UpdateStress(m_matchCtrl.GetPlayerScore(id), m_matchCtrl.GetPlayerScore(GameMode::GetOpposite(id)));
			}

			object->Update(deltaTime);
		}

		DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
		DECL_GET_OR_RETURN(camera, gameMgr->GetCamera());

		if (GameMode::s_type == vsWall)
			m_matchCtrl.SetPlayerScore(Player1, ball->GetRallieCount());

		if (!camera->IsInView(ball->GetVolume()))
		{
			ball->SetLastRallieCount();

			if (ball->GetPosition().x < 0)
			{
				m_matchCtrl.IncrementPlayerScore(Player2);
			}
			else if (ball->GetPosition().x > GameConstants::ScreenDim.x)
			{
				m_matchCtrl.IncrementPlayerScore(Player1);
			}

			for (auto& [_, object] : m_objects)
			{
				CONTINUE_IF_INVALID(object);
				object->Reset();
			}
		}
	}
}

void GameCourt::ResetScene()
{
	m_matchCtrl.Reset();
	IScene::ResetScene();
}

void GameCourt::SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos)
{
}

bool GameCourt::AddEnemies()
{
	return true;
}

bool GameCourt::AddGUI()
{
	float charSize = GameConstants::ScreenDim.y * 0.08f;

	m_matchCtrl.PlacePlayersScoreCard(Player1, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.167f, charSize * 0.75f));
	m_matchCtrl.PlacePlayersScoreCard(Player2, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.833f, charSize * 0.75f));

	return true;
}

bool GameCourt::AddObjects()
{
	EmplaceObjectOrThrow<Ball>("Ball", GameConstants::ScreenDim * 0.5f);

	switch (GameMode::s_type)
	{
	case VsAI:
		EmplaceObjectOrThrow<Player>("FirstPlayer", GameMode::s_player1);
		EmplaceObjectOrThrow<AutomatedPlayer>("SecondPlayer", GameMode::s_player2);
		break;
	case VsPly:
		EmplaceObjectOrThrow<Player>("FirstPlayer", GameMode::s_player1);
		EmplaceObjectOrThrow<Player>("SecondPlayer", GameMode::s_player2);
		break;
	case AIvsAI:
		EmplaceObjectOrThrow<AutomatedPlayer>("FirstPlayer", GameMode::s_player1);
		EmplaceObjectOrThrow<AutomatedPlayer>("SecondPlayer", GameMode::s_player2);
		break;
	}

	m_ballPhysics.SetCurrentBall(dynamic_cast<Ball*>(GetObjectByName("Ball")));

	return true;
}

bool GameCourt::AddForeGroundObjects()
{
	auto scrCen = GameConstants::ScreenDim * 0.5f;
	auto halfWallThick = DifficultyMode::wallThickness * 0.5f;

	EmplaceObjectOrThrow<Wall>("TopWall", SetHorizontalWall(Vector2f(scrCen.x, halfWallThick)));
	EmplaceObjectOrThrow<Wall>("BottomWall", SetHorizontalWall(Vector2f(scrCen.x, GameConstants::ScreenDim.y - halfWallThick)));

	return true;
}


void GameCourt::UpdateGUI(float deltaTime)
{
	m_matchCtrl.Update(deltaTime);
}

void GameCourt::RenderGUI(IRenderer* renderer)
{
	ENSURE_VALID(renderer);

	m_matchCtrl.Render(renderer);
}