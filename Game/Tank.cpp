#include "Tank.h"
#include "Sprite.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Utils/Math/Math.h"
#include "../Source/Physics/Body.h"
#include "../Source/Physics/BoxCollider.h"
#include "../Source/Physics/CircleCollider.h"
#include "../Source/Physics/World.h"


namespace GameDev2D
{
    Tank::Tank(TankColor aTankColor) : GameObject("Tank"),
        m_TankColor(aTankColor),
        m_Body(nullptr),
        m_Turret(nullptr),
        m_PhysicsBody(nullptr),
        m_FireDelay(0.0)
    {
        //Initialize the Tank's position and angle based on its color
        SetPosition(TANK_STARTING_POSITION[m_TankColor]);
        SetAngle(TANK_STARTING_ANGLE[m_TankColor]);

        //Initialize the input array
        for (int i = 0; i < TankInputCount; i++)
        {
            m_Input[i] = TANK_DEFAULT_INPUT[i];
        }

        //Create the 'Tank' Sprite object
		if (m_TankColor == TankBlue)
			m_Body = new Sprite("SpriteAtlas", "Tank-Blue");

		if (m_TankColor == TankGreen)
			m_Body = new Sprite("SpriteAtlas", "Tank-Green");

		if (m_TankColor == TankExploded)
			m_Body = new Sprite("SpriteAtlas", "Tank-Exploded");

        m_Body->SetAnchorPoint(0.5f, 0.5f);
        AddChild(m_Body);

        //Create the 'Turret' Sprite object
		if (m_TankColor != TankExploded)
			m_Turret = new Sprite("SpriteAtlas", "Turret");
		else
			m_Turret = new Sprite("SpriteAtlas", "Turret-Exploded");

        m_Turret->SetAnchorPoint(0.2f, 0.5f);
        AddChild(m_Turret);

        //Initialize the parameters to create the Physics body
        float density = 2.0f;
        float width = Math::PixelsToMeters(GetWidth());
        float height = Math::PixelsToMeters(GetHeight());
        Physics::BoxCollider* boxCollider = new Physics::BoxCollider(width, height);

        //Create the Physics body to attach to the tank
        m_PhysicsBody = Physics::World::GetInstance()->CreateBody(boxCollider, density);
        m_PhysicsBody->SetGameObject(this);
        m_PhysicsBody->SetPosition(Math::PixelsToMeters(GetWorldPosition()));
        m_PhysicsBody->SetAngle(Math::DegreesToRadians(GetWorldAngle()));
        m_PhysicsBody->SetLinearDamping(vec2(2.0f, 2.0f));
        m_PhysicsBody->SetAngularDamping(0.6f);
        m_PhysicsBody->SetMass(5);
    }

    Tank::~Tank()
    {
        m_Body = nullptr;
        m_Turret = nullptr;
        m_PhysicsBody = nullptr;
    }

    void Tank::Update(double aDelta)
    {
        if (m_PhysicsBody == nullptr)
        {
            return;
        }

        //If there's a fire delay, count down the 
        //fire delay timer by delta time
        if (m_FireDelay > 0.0)
        {
            m_FireDelay -= aDelta;
            if (m_FireDelay <= 0.0)
            {
                m_FireDelay = 0.0;
            }
        }

        //Implement in-class assignment HERE!
        //First calculate the tank's rotated normal
        vec2 lateralNormal = vec2(0, 1);
        float c = cosf(GetAngle());
        float s = sinf(GetAngle());
        vec2 rotatedNormal;
        rotatedNormal.x = c * lateralNormal.x - s * lateralNormal.y;
        rotatedNormal.y = s * lateralNormal.x + c * lateralNormal.y;

        //Next calculate the tank's lateral velocity vector
        float magnitude = Math::Dot(rotatedNormal,  GetLinearVelocity());
        vec2 lateralVelocity = rotatedNormal * magnitude;

        //Next use impulses to cancel out the lateral velocity
        vec2 impulse = m_PhysicsBody->GetMass() * -lateralVelocity; 
        //vec2 impulse = vec2(2, 2) * -lateralVelocity;
        m_PhysicsBody->ApplyLinearImpulse(impulse);

        //Use Impulses to cancel out the angular velocity
        float angularImpulse = 0.15f * m_PhysicsBody->GetInertia() * -(m_PhysicsBody->GetAngularVelocity());
        m_PhysicsBody->ApplyAngularImpulse(angularImpulse);

        //Handle the turret rotation angle
        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputTurretLeft]) == true)
        {
            m_Turret->IncreaseAngle(TURRET_ANGULAR_SPEED * aDelta);
        }
        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputTurretRight]) == true)
        {
            m_Turret->DecreaseAngle(TURRET_ANGULAR_SPEED * aDelta);
        }

        //Reset the tank's speed
        float force = 0.0f;

        //Handle the tank's speed controls
        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputUp]) == true)
        {
            force += TANK_FORCE_AMOUNT;
        }

        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputDown]) == true)
        {
            force -= TANK_FORCE_AMOUNT;
        }
        
        //Apply the force to the physics body 
        vec2 force2d = vec2(cosf(m_PhysicsBody->GetAngle()) * force, sinf(m_PhysicsBody->GetAngle()) * force);
        m_PhysicsBody->ApplyForce(force2d);

        //Declare a variable for torque
        float torque = 0.0f;

        //Handle the tank's rotation controls
        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputLeft]) == true)
        {
            torque += TANK_TORQUE_AMOUNT;
        }

        if (ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputRight]) == true)
        {
            torque -= TANK_TORQUE_AMOUNT;
        }

        //Apply the torque to the physics body 
        if (force != 0.0)
        {
            m_PhysicsBody->ApplyTorque(torque);
        }

        GameObject::Update(aDelta);
    }

    float Tank::GetWidth()
    {
        return m_Body->GetWidth();
    }

    float Tank::GetHeight()
    {
        return m_Body->GetHeight();
    }

    void Tank::SetLeftInput(KeyCode aLeftInput)
    {
        m_Input[TankInputLeft] = aLeftInput;
    }

    void Tank::SetRightInput(KeyCode aRightInput)
    {
        m_Input[TankInputRight] = aRightInput;
    }

    void Tank::SetUpInput(KeyCode aUpInput)
    {
        m_Input[TankInputUp] = aUpInput;
    }

    void Tank::SetDownInput(KeyCode aDownInput)
    {
        m_Input[TankInputDown] = aDownInput;
    }

    void Tank::SetTurretLeftInput(KeyCode aLeftTurretInput)
    {
        m_Input[TankInputTurretLeft] = aLeftTurretInput;
    }

    void Tank::SetTurretRightInput(KeyCode aRightTurretInput)
    {
        m_Input[TankInputTurretRight] = aRightTurretInput;
    }

    void Tank::SetFireTurretInput(KeyCode aFireTurretInput)
    {
        m_Input[TankInputFireTurret] = aFireTurretInput;
    }

    vec2 Tank::GetShellPosition()
    {
        //Calculate the starting position of the shell
        float turretLength = m_Turret->GetWidth() * (1.0f - m_Turret->GetAnchorPointX());
        vec2 offset = m_Turret->GetWorldDirection() * vec2(turretLength, turretLength);
        vec2 position = m_Turret->GetWorldPosition();
        return position + offset;
    }

	Physics::Body* Tank::GetPhysicsBody()
	{
		return m_PhysicsBody;
	}

    float Tank::GetTurretAngle()
    {
        return m_Turret->GetWorldAngle();
    }

	void Tank::SetTurrentAngle(float aAngle)
	{
		m_Turret->SetAngle(aAngle);
	}

    bool Tank::CanFireShell()
    {
        return ServiceLocator::GetInputManager()->IsKeyPressed(m_Input[TankInputFireTurret]) && m_FireDelay <= 0.0;
    }

    void Tank::ResetFireDelay()
    {
        m_FireDelay = TANK_FIRE_DELAY;
    }
}