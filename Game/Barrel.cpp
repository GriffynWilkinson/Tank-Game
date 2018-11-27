#include "Barrel.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Utils/Math/Math.h"
#include "../Source/Physics/Body.h"
#include "../Source/Physics/BoxCollider.h"
#include "../Source/Physics/CircleCollider.h"
#include "../Source/Physics/World.h"


namespace GameDev2D
{
    Barrel::Barrel() : GameObject("Barrel"),
        m_Barrel(nullptr),
        m_PhysicsBody(nullptr)
    {
        //Create the Sprite and add it as a child
        m_Barrel = new Sprite("SpriteAtlas", "Barrel");
        m_Barrel->SetAnchorPoint(0.5f, 0.5f);
        AddChild(m_Barrel);

        //
        m_Explosion = new AnimatedSprite();
        m_Explosion->SetIsEnabled(false);
        m_Explosion->SetFrameSpeed(30.0f);
        m_Explosion->SetDoesLoop(false);
        m_Explosion->SetAnchorPoint(0.5f, 0.5f);
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-1");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-2");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-3");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-4");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-5");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-6");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-7");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-8");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-9");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-10");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-11");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-12");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-13");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-14");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-15");
        m_Explosion->AddFrame("SpriteAtlas", "Explosion-16");
        AddChild(m_Explosion);

        //Initialize the parameters to create the Physics body
        float density = 1.0f;
        float radius = Math::PixelsToMeters(GetHeight() / 2.0f);
        Physics::CircleCollider* circleCollider = new Physics::CircleCollider(radius);

        //Create the Physics body to attach to the tank
        m_PhysicsBody = Physics::World::GetInstance()->CreateBody(circleCollider, density);
        m_PhysicsBody->SetGameObject(this);
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(vec2(WINDOWS_DEFAULT_WIDTH / 2, WINDOWS_DEFAULT_HEIGHT / 2)));
        m_PhysicsBody->SetLinearDamping(vec2(0.2f, 0.2));
        m_PhysicsBody->SetAngularDamping(1.0f);
    }

    Barrel::~Barrel()
    {
        m_Barrel = nullptr;
        m_PhysicsBody = nullptr;
    }

    void Barrel::SetX(float x)
    {
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(vec2(x, GetY())));
        GameObject::SetX(x);
    }

    void Barrel::SetY(float y)
    {
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(vec2(GetX(), y)));
        GameObject::SetY(y);
    }

    void Barrel::SetPosition(float x, float y)
    {
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(vec2(x, y)));
        GameObject::SetPosition(x,y);
    }

    void Barrel::SetPosition(vec2 position)
    {
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(position));

        GameObject::SetPosition(position);
    }

    float Barrel::GetWidth()
    {
        return m_Barrel->GetWidth();
    }

    float Barrel::GetHeight()
    {
        return m_Barrel->GetHeight();
    }

    void Barrel::Explode()
    {
        if (HasExploded() == false)
        {
            m_Explosion->SetFrameIndex(0);
            m_Explosion->SetIsEnabled(true);
            m_Barrel->SetIsEnabled(false);
        }
    }

    bool Barrel::HasExploded()
    {
        return m_Barrel->IsEnabled() == false;
    }
}