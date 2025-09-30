#include "GameCourt.h"

#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Player.h"
#include "../../GameObjects/Wall.h"
#include "../../Utilities/DifficultyMode.h"
#include "../../Utilities/GameMode.h"
#include "../../Engine/Core/PongFramework.h"
#include <Drawables/SFSprite.h>
#include <Engine/Core/GameManager.h>

GameCourt::GameCourt()
{
	m_backgroundSpr = std::make_shared<SFSprite>("Court");
	auto bkgSpr = dynamic_cast<SFSprite*>(m_backgroundSpr.get());
	if (bkgSpr)
	{
		bkgSpr->SetScale(GameConstants::Scale);
		bkgSpr->SetOrigin(GameConstants::ScreenDim / 2.f);
		bkgSpr->SetPosition(GameConstants::ScreenDim / 2.f);
	}
}

void GameCourt::Update(float deltaTime)
{
	UpdateGUI(deltaTime);

	auto ball = static_cast<Ball*>(GetObjectByName("Ball"));
	if (!ball)
		return;

	if (m_matchCtrl.IsReady() && !m_matchCtrl.IsOver())
	{
		Player* ply = nullptr;

		for (auto& [name, object] : m_objects)
		{
			if (!object->GetActive())
				continue;

			if (name.ends_with("Player"))
			{
				if (Player* ply = dynamic_cast<Player*>(object.get()))
				{
					m_matchCtrl.SetCurrentMatchResults(ply->GetPlayerID());
					if (AutomatedPlayer* aiPly = dynamic_cast<AutomatedPlayer*>(ply))
					{
						aiPly->UpdateFatigue(ball->GetRallieCount());
						auto id = aiPly->GetPlayerID();
						aiPly->UpdateStress(m_matchCtrl.GetPlayerScore(id), m_matchCtrl.GetPlayerScore(GameMode::GetOpposite(id)));

					}
				}
			}

			object->Update(deltaTime);
		}

		auto camera = GameManager::Get()->GetCamera();
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

void GameCourt::AddGUI()
{
	float charSize = GameConstants::ScreenDim.y * 0.08f;

	m_matchCtrl.PlacePlayersScoreCard(Player1, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.167f, charSize * 0.75f));
	m_matchCtrl.PlacePlayersScoreCard(Player2, (int)charSize, Vector2f(GameConstants::ScreenDim.x * 0.833f, charSize * 0.75f));
}

void GameCourt::AddEnemies()
{

}

void GameCourt::AddObjects()
{
	m_objects.emplace("Ball", std::make_shared<Ball>(Vector2f(GameConstants::ScreenDim / 2.0f)));

	switch (GameMode::s_type)
	{
	case VsAI:
		m_objects.emplace("FirstPlayer", std::make_shared<Player>(GameMode::s_player1));
		m_objects.emplace("SecondPlayer", std::make_shared<AutomatedPlayer>(GameMode::s_player2));
		break;
	case VsPly:
		m_objects.emplace("FirstPlayer", std::make_shared<Player>(GameMode::s_player1));
		m_objects.emplace("SecondPlayer", std::make_shared<Player>(GameMode::s_player2));
		break;
	case AIvsAI:
		m_objects.emplace("FirstPlayer", std::make_shared<AutomatedPlayer>(GameMode::s_player1));
		m_objects.emplace("SecondPlayer", std::make_shared<AutomatedPlayer>(GameMode::s_player2));
		break;
	}

	m_ballPhysics.SetCurrentBall(dynamic_cast<Ball*>(GetObjectByName("Ball")));
}

void GameCourt::AddForeGroundObjects()
{
	m_objects.emplace("TopWall", std::make_shared<Wall>(SetHorizontalWall(Vector2f(GameConstants::ScreenDim.x / 2.f, DifficultyMode::wallThickness / 2.f))));
	m_objects.emplace("BottomWall", std::make_shared<Wall>(SetHorizontalWall(Vector2f(GameConstants::ScreenDim.x / 2.f, GameConstants::ScreenDim.y - (DifficultyMode::wallThickness / 2.f)))));
}

void GameCourt::SpawnGameObjectAt(const std::string& id, std::shared_ptr<GameObject> obj, const Vector2f& pos)
{

}

void GameCourt::UpdateGUI(float deltaTime)
{
	m_matchCtrl.Update(deltaTime);
}

void GameCourt::RenderGUI(IRenderer* renderer)
{
	m_matchCtrl.Render(renderer);
}