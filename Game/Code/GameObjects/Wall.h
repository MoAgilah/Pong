#pragma once

#include <GameObjects/GameObject.h>
#include <Utilities/Vector.h>
#include <string>

enum WallOrientation
{
	Horizontal,
	Vertical
};

struct WallConfig
{
	std::string m_texID;
	Vector2f m_pos;
	Vector2f m_size;
	Vector2f m_scale;
	WallOrientation m_orientation;
};

WallConfig SetHorizontalWall(const Vector2f& pos);

WallConfig SetVerticalWall(const Vector2f& pos);

class Wall : public GameObject
{
public:
	Wall(const WallConfig& config);
	~Wall() = default;

	void Update(float deltaTime) override;

	void OnCollisionEnter(IGameObject* obj) override;
	void OnCollisionStay(IGameObject* obj) override;
	void OnCollisionExit(IGameObject* obj) override;

	void ResolveCollisions(float time, const Vector2f& sepVec, float relHitPos) override;

	WallOrientation GetOrientation() const { return m_orientation; }

private:

	WallOrientation m_orientation;
};