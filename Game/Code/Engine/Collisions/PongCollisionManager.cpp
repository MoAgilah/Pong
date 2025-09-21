#include "PongCollisionManager.h"

#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Player.h"
#include "../../GameObjects/Wall.h"

PongCollisionManager::PongCollisionManager()
    : ICollisionManager(nullptr)
{
}

void PongCollisionManager::DynamicObjectToObjectResolution(IDynamicGameObject* obj1, IGameObject* obj2, float time)
{
    Vector2f sepVec = obj2->GetVolume()->GetSeparationVector(obj1->GetVolume());
    Vector2f normal;

    if (Ball* ball = dynamic_cast<Ball*>(obj1))
    {
        if (Wall* wall = dynamic_cast<Wall*>(obj2))
        {
            if (!ball->RallyHasBegun())
            {
                if (wall->GetOrientation() == WallOrientation::Vertical)
                {
                    float relHitPos = (ball->GetPosition().y - wall->GetPosition().y) /
                        (wall->GetDrawable()->GetSize().x / 2.0f);

                    relHitPos = std::clamp(relHitPos, -1.f, 1.f);

                    // Enable only if it's far enough from center
                    if (std::abs(relHitPos) > 0.128f)
                        ball->BeginRally();
                }
            }

            // If no separation vector, infer from velocity or position
            if (sepVec == Vector2f(0.f, 0.f))
            {
                Vector2f vel = ball->GetVelocity();

                if (wall->GetOrientation() == WallOrientation::Horizontal)
                {
                    if (std::abs(vel.y) > ICollisionManager::EPSILON)
                    {
                        normal = Vector2f(0.f, (vel.y > 0.f) ? -1.f : 1.f);
                    }
                    else
                    {
                        // Infer from position difference
                        float dy = ball->GetPosition().y - wall->GetPosition().y;
                        normal = Vector2f(0.f, (dy >= 0.f) ? 1.f : -1.f);
                    }
                }
                else // Vertical wall
                {
                    if (std::abs(vel.x) > ICollisionManager::EPSILON)
                    {
                        normal = Vector2f((vel.x > 0.f) ? -1.f : 1.f, 0.f);
                    }
                    else
                    {
                        // Infer from position difference
                        float dx = ball->GetPosition().x - wall->GetPosition().x;
                        normal = Vector2f((dx >= 0.f) ? 1.f : -1.f, 0.f);
                    }
                }

                auto circle = dynamic_cast<IBoundingCircle*>(ball->GetVolume());
                if (circle)
                {
                    float pushOut = circle->GetRadius() + 0.01f;
                    sepVec = normal * pushOut;
                }
            }
            else
            {
                normal = sepVec.Normalize();
            }

            // Move ball
            Vector2f velocity = ball->GetVelocity();
            Vector2f newPos = ball->GetPosition() + velocity * time + sepVec;
            ball->SetPosition(newPos);
            ball->GetVolume()->Update(newPos);

            // Reflect ball using inferred/applied normal
            if (normal != Vector2f(0.f, 0.f))
            {
                ball->ReflectWall(normal);
            }
        }
    }

    // Handle paddle-to-wall collisions
    if (Player* ply = dynamic_cast<Player*>(obj1))
    {
        if (Wall* wall = dynamic_cast<Wall*>(obj2))
        {
            ply->ResolveCollisions(time, sepVec, 0);
        }
    }
}

void PongCollisionManager::DynamicObjectToDynamicObjectResolution(IDynamicGameObject* obj1, IDynamicGameObject* obj2, float time)
{
    Vector2f sepVec = obj1->GetVolume()->GetSeparationVector(obj2->GetVolume());
    if (Ball* ball = dynamic_cast<Ball*>(obj1))
    {
        if (Player* paddle = dynamic_cast<Player*>(obj2))
        {
            auto capsule = dynamic_cast<IBoundingCapsule*>(paddle->GetVolume());
            if (capsule)
            {
                float relHitPos = (ball->GetPosition().y - paddle->GetPosition().y) /
                    (capsule->GetLength() / 2.0f);
                relHitPos = std::clamp(relHitPos, -1.f, 1.f);

                ball->SetPaddleSpeed(paddle->GetYVelocity());
                ball->ResolveCollisions(time, sepVec, relHitPos);
            }
        }
    }
}