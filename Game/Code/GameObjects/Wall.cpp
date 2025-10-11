#include "Wall.h"

#include "../Utilities/DifficultyMode.h"
#include <Drawables/SFShape.h>
#include <Drawables/SFSprite.h>
#include <Engine/Collisions/BoundingBox.h>
#include <Engine/Core/Constants.h>
#include <Engine/Core/GameManager.h>
#include <Utilities/Utils.h>

WallConfig SetHorizontalWall(const Vector2f& pos)
{
	WallConfig wc;
	wc.m_texID = "Wall";
	wc.m_pos = pos;
	wc.m_size = Vector2f(128.f, 24.f);
	wc.m_scale = Vector2f(GameConstants::ScreenDim.x / 128.f, DifficultyMode::wallThickness / 24.f);
	wc.m_orientation = Horizontal;
	return wc;
}

WallConfig SetVerticalWall(const Vector2f& pos)
{
	WallConfig wc;
	wc.m_texID = "SideWall";
	wc.m_pos = Vector2f(pos.x, pos.y + DifficultyMode::wallThickness);
	wc.m_size = Vector2f(24.f, 128.f);
	wc.m_scale = Vector2f(DifficultyMode::wallThickness / 24.f, (GameConstants::ScreenDim.y - (DifficultyMode::wallThickness * 2.f)) / 128.f);
	wc.m_orientation = Vertical;
	return wc;
}

Wall::Wall(const WallConfig& config)
	: GameObject(std::make_shared<SFSprite>(config.m_texID),std::make_shared<BoundingBox<SFRect>>(config.m_size, Vector2f())),
	m_orientation(config.m_orientation)
{
	SetInitialPosition(config.m_pos);
	SetPosition(GetInitialPosition());

	SetScale(config.m_scale);

	ENSURE_VALID(m_volume);
	m_volume->Update(GetPosition());
}

void Wall::Update(float deltaTime)
{
	DECL_GET_OR_RETURN(gameMgr, GameManager::Get());
	DECL_GET_OR_RETURN(colMgr, gameMgr->GetCollisionMgr());

	colMgr->ProcessCollisions(this);
}

void Wall::OnCollisionEnter(IGameObject* obj)
{
	ENSURE_VALID(obj);
}

void Wall::OnCollisionStay(IGameObject* obj)
{
	ENSURE_VALID(obj);
}

void Wall::OnCollisionExit(IGameObject* obj)
{
	ENSURE_VALID(obj);
}

void Wall::ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos)
{
}