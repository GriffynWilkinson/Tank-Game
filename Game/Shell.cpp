#include "Shell.h"
#include "Sprite.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Utils/Math/Math.h"
#include "../Source/Physics/Body.h"
#include "../Source/Physics/BoxCollider.h"
#include "../Source/Physics/CircleCollider.h"
#include "../Source/Physics/World.h"


namespace GameDev2D
{
    Shell::Shell() : GameObject("Shell"), 
        m_Sprite(nullptr),
        m_PhysicsBody(nullptr)
    {
        //Create the Sprite and add it as a child
        m_Sprite = new Sprite("SpriteAtlas", "Shell");
        m_Sprite->SetAnchorPoint(1.0f, 0.5f);
        AddChild(m_Sprite);

        //Initialize the Shell to be disabled
        SetIsEnabled(false);

        //Initialize the parameters to create the Physics body
        float density = 1.0f;
        float radius = Math::PixelsToMeters(GetHeight() / 2.0f);
        Physics::CircleCollider* circleCollider = new Physics::CircleCollider(radius);

        //Create the Physics body to attach to the tank
        m_PhysicsBody = Physics::World::GetInstance()->CreateBody(circleCollider, density);
        m_PhysicsBody->SetGameObject(this);
        m_PhysicsBody->SetLinearDamping(vec2(0.2f, 0.2));
        m_PhysicsBody->SetAngularDamping(1.0f);
    }

    Shell::~Shell()
    {
        m_Sprite = nullptr;
        m_PhysicsBody = nullptr;
    }

    void Shell::Update(double aDelta)
    {
        //If the shell is enabled, update it and check it's bounds
        if (IsEnabled() == true)
        {
            //Get the screen width and height 
            float screenWidth = ServiceLocator::GetPlatformLayer()->GetWidth();
            float screenHeight = ServiceLocator::GetPlatformLayer()->GetHeight();

            //If the shell went off-screen, disable it
            if (GetX() < 0.0f || GetX() > screenWidth || GetY() < 0.0f || GetY() > screenHeight)
            {
                SetIsEnabled(false);
            }
        }

        //This call below calls the Base class' Update method, the base class 
        //being GameObject, its important so that the children get updated
        GameObject::Update(aDelta);
    }

    float Shell::GetWidth()
    {
        return m_Sprite->GetWidth();
    }

    float Shell::GetHeight()
    {
        return m_Sprite->GetHeight();
    }

    void Shell::Fire(vec2 aPosition, float aAngleInDegrees)
    {
        //Enable the shell
        SetIsEnabled(true);

        //Is there a physics Body set?
        if (m_PhysicsBody != nullptr)
        {
            //Reset the Body's linear and angular velocities
            m_PhysicsBody->SetLinearVelocity(vec2(0.0f, 0.0f));
            m_PhysicsBody->SetAngularVelocity(0.0f);

            //Set the Shell's position, angle and linear velocity
            m_PhysicsBody->SetPosition(Math::PixelsToMeters(aPosition));

            //
            float angleInRadians = Math::DegreesToRadians(aAngleInDegrees);
            m_PhysicsBody->SetAngle(angleInRadians);

            //
            vec2 force2d = vec2(cosf(angleInRadians) * SHELL_FORCE, sinf(angleInRadians) * SHELL_FORCE);
            m_PhysicsBody->ApplyForce(force2d);
        }
        else
        {
            SetPosition(aPosition);
            SetAngle(aAngleInDegrees);
            SetLinearVelocity(Math::CalculateLinearVelocity(SHELL_SPEED, aAngleInDegrees));
        }
    }
}