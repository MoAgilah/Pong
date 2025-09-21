#pragma once

#include <Engine/Interface/Collisions/ICollisionManager.h>

class PongCollisionManager : public ICollisionManager
{
public:
	PongCollisionManager();
	~PongCollisionManager() = default;

private:

	void DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time) override;
	void DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time) override;
};