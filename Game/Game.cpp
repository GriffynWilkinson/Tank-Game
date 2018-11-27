//
//  Game.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#include "Game.h"
#include "Label.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Audio.h"
#include "Polygon.h"
#include "Tank.h"
#include "Shell.h"
#include "Barrel.h"

#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"
#include "../Source/Graphics/Graphics.h"
#include "../Source/Events/Input/InputEvents.h"
#include "../Source/Events/Platform/ResizeEvent.h"
#include "../Source/Utils/Math/Math.h"

#include "../Source/Physics/Body.h"
#include "../Source/Physics/World.h"
#include "../Source/Physics/CircleCollider.h"
#include "../Source/Physics/BoxCollider.h"


namespace GameDev2D
{
    Game::Game() : BaseObject("Game"),
        m_Camera(nullptr),
        m_TimeStep(1.0 / (double)ServiceLocator::GetPlatformLayer()->GetTargetFramerate()),
        m_ScreenWidth(0.0f),
        m_ScreenHeight(0.0f)
    {
        //Create the camera object
        m_Camera = new Camera();

        //Set the physics world's listener
        Physics::World::GetInstance()->SetListener(this);

        //Create the first Tank object
        m_BlueTank = new Tank(TankBlue);
        m_BlueTank->SetLeftInput(KEY_CODE_A);
        m_BlueTank->SetRightInput(KEY_CODE_D);
        m_BlueTank->SetUpInput(KEY_CODE_W);
        m_BlueTank->SetDownInput(KEY_CODE_S);
        m_BlueTank->SetTurretLeftInput(KEY_CODE_Q);
        m_BlueTank->SetTurretRightInput(KEY_CODE_E);
        m_BlueTank->SetFireTurretInput(KEY_CODE_TAB);

        //Create the second Tank object
        m_GreenTank = new Tank(TankGreen);
        m_GreenTank->SetLeftInput(KEY_CODE_LEFT);
        m_GreenTank->SetRightInput(KEY_CODE_RIGHT);
        m_GreenTank->SetUpInput(KEY_CODE_UP);
        m_GreenTank->SetDownInput(KEY_CODE_DOWN);
        m_GreenTank->SetTurretLeftInput(KEY_CODE_N);
        m_GreenTank->SetTurretRightInput(KEY_CODE_M);
        m_GreenTank->SetFireTurretInput(KEY_CODE_SPACE);

        m_ExplodedTank = new Tank(TankExploded);
        m_ExplodedTank->SetIsEnabled(false);

        //Create the Shell objects
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            m_Shells[i] = new Shell();
        }

        m_Background = new Sprite("Background.png");
        m_Background->SetPosition(WINDOWS_DEFAULT_WIDTH / 2, WINDOWS_DEFAULT_HEIGHT / 2);
        m_Background->SetAnchorPoint(0.5f, 0.5f);

        //Create the Barrel object
        for (unsigned int i = 0; i < AMOUNT_OF_BARRELS; i++)
        {
            m_Barrel[i] = new Barrel();
            m_Barrel[i]->SetAnchorPoint(0.5f, 0.5f);
        }

        //Position the Barrels
        m_Barrel[0]->SetPosition(vec2(225.0f, 587.5f));
        m_Barrel[1]->SetPosition(vec2(225.0f, 462.5f));
        m_Barrel[2]->SetPosition(vec2(225.0f, 337.5f));
        m_Barrel[3]->SetPosition(vec2(225.0f, 212.5f));

        m_Barrel[4]->SetPosition(vec2(287.5f, 650.0f));
        m_Barrel[5]->SetPosition(vec2(287.5f, 525.0f));
        m_Barrel[6]->SetPosition(vec2(287.5f, 400.0f));
        m_Barrel[7]->SetPosition(vec2(287.5f, 275.0f));
        m_Barrel[8]->SetPosition(vec2(287.5f, 150.0f));

        m_Barrel[9]->SetPosition(vec2(350.0f, 587.5f));
        m_Barrel[10]->SetPosition(vec2(350.0f, 462.5f));
        m_Barrel[11]->SetPosition(vec2(350.0f, 337.5f));
        m_Barrel[12]->SetPosition(vec2(350.0f, 212.5f));

        m_Barrel[13]->SetPosition(vec2(412.5f, 650.0f));
        m_Barrel[14]->SetPosition(vec2(412.5f, 525.0f));
        m_Barrel[15]->SetPosition(vec2(412.5f, 400.0f));
        m_Barrel[16]->SetPosition(vec2(412.5f, 275.0f));
        m_Barrel[17]->SetPosition(vec2(412.5f, 150.0f));

        m_Barrel[18]->SetPosition(vec2(475.0f, 462.5f));
        m_Barrel[19]->SetPosition(vec2(475.0f, 337.5f));

        m_Barrel[20]->SetPosition(vec2(537.5f, 525.0f));
        m_Barrel[21]->SetPosition(vec2(537.5f, 400.0f));
        m_Barrel[22]->SetPosition(vec2(537.5f, 272.0f));

        m_Barrel[23]->SetPosition(vec2(600.0f, 462.5f));
        m_Barrel[24]->SetPosition(vec2(600.0f, 337.5f));

        m_Barrel[25]->SetPosition(vec2(662.5f, 525.0f));
        m_Barrel[26]->SetPosition(vec2(662.5f, 400.0f));
        m_Barrel[27]->SetPosition(vec2(662.5f, 272.0f));

        m_Barrel[28]->SetPosition(vec2(725.0f, 462.5f));
        m_Barrel[29]->SetPosition(vec2(725.0f, 337.5f));

        m_Barrel[30]->SetPosition(vec2(787.5f, 650.0f));
        m_Barrel[31]->SetPosition(vec2(787.5f, 525.0f));
        m_Barrel[32]->SetPosition(vec2(787.5f, 400.0f));
        m_Barrel[33]->SetPosition(vec2(787.5f, 275.0f));
        m_Barrel[34]->SetPosition(vec2(787.5f, 150.0f));

        m_Barrel[35]->SetPosition(vec2(850.0f, 587.5f));
        m_Barrel[36]->SetPosition(vec2(850.0f, 462.5f));
        m_Barrel[37]->SetPosition(vec2(850.0f, 337.5f));
        m_Barrel[38]->SetPosition(vec2(850.0f, 212.5f));

        m_Barrel[39]->SetPosition(vec2(912.5f, 650.0f));
        m_Barrel[40]->SetPosition(vec2(912.5f, 525.0f));
        m_Barrel[41]->SetPosition(vec2(912.5f, 400.0f));
        m_Barrel[42]->SetPosition(vec2(912.5f, 275.0f));
        m_Barrel[43]->SetPosition(vec2(912.5f, 150.0f));

        m_Barrel[44]->SetPosition(vec2(975.0f, 587.5f));
        m_Barrel[45]->SetPosition(vec2(975.0f, 462.5f));
        m_Barrel[46]->SetPosition(vec2(975.0f, 337.5f));
        m_Barrel[47]->SetPosition(vec2(975.0f, 212.5f));

        //Create the Detonator Objects
        m_BlueDetonator = new Sprite("SpriteAtlas", "Detonator-Blue");
        m_BlueDetonator->SetAnchorPoint(0.5f, 0.5f);
        m_BlueDetonator->SetIsEnabled(false);

        m_GreenDetonator = new Sprite("SpriteAtlas", "Detonator-Green");
        m_GreenDetonator->SetAnchorPoint(0.5f, 0.5f);
        m_GreenDetonator->SetIsEnabled(false);

        int blueRand = rand() % (AMOUNT_OF_BARRELS - 1);
        int greenRand;
        do
        {
        greenRand = rand() % (AMOUNT_OF_BARRELS - 1);
        } while (greenRand == blueRand);

        //int blueRand = 23;
        //int greenRand = 24;

        m_BlueDetonator->SetPosition(m_Barrel[blueRand]->GetPosition());
        m_GreenDetonator->SetPosition(m_Barrel[greenRand]->GetPosition());

        //Initialize the parameters to create the Physics body;
        float density = 1.0f;
        float radius = Math::PixelsToMeters(m_BlueDetonator->GetHeight() / 2.0f);
        Physics::CircleCollider* circleCollider = new Physics::CircleCollider(radius);

        //Create the Physics body to attach to the detonators;
        Physics::Body* physicsBodyBlue = Physics::World::GetInstance()->CreateBody(circleCollider, density);
        physicsBodyBlue->SetGameObject(m_BlueDetonator);
        physicsBodyBlue->SetPosition(Math::PixelsToMeters(m_BlueDetonator->GetPosition()));
        physicsBodyBlue->SetLinearDamping(vec2(0.2f, 0.2f));
        physicsBodyBlue->SetAngularDamping(1.0f);

        Physics::Body* physicsBodyGreen = Physics::World::GetInstance()->CreateBody(circleCollider, density);
        physicsBodyGreen->SetGameObject(m_GreenDetonator);
        physicsBodyGreen->SetPosition(Math::PixelsToMeters(m_GreenDetonator->GetPosition()));
        physicsBodyGreen->SetLinearDamping(vec2(0.2f, 0.2f));
        physicsBodyGreen->SetAngularDamping(1.0f);

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
    }

    Game::~Game()
    {
        if (m_Camera != nullptr)
        {
            delete m_Camera;
            m_Camera = nullptr;
        }

        if (m_BlueTank != nullptr)
        {
            delete m_BlueTank;
            m_BlueTank = nullptr;
        }

        if (m_GreenTank != nullptr)
        {
            delete m_GreenTank;
            m_GreenTank = nullptr;
        }

        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            delete m_Shells[i];
            m_Shells[i] = nullptr;
        }

        for (unsigned int i = 0; i < AMOUNT_OF_BARRELS; i++)
        {
            delete m_Barrel[i];
            m_Barrel[i] = nullptr;
        }

        if (m_Background != nullptr)
        {
            delete m_Background;
            m_Background = nullptr;
        }

        if (m_ExplodedTank != nullptr)
        {
            delete m_ExplodedTank;
            m_ExplodedTank = nullptr;
        }

        if (m_Explosion != nullptr)
        {
            delete m_Explosion;
            m_Explosion = nullptr;
        }
    }

    void Game::Update(double aDelta)
    {
        //Update the Tanks
        UpdateTank(m_BlueTank, aDelta);
        UpdateTank(m_GreenTank, aDelta);

        m_Explosion->Update(aDelta);

        //Step the Physics World, it needs a 'fixed' time step
        Physics::World::GetInstance()->Step(m_TimeStep);

        //Update the Shells
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (m_Shells[i]->IsEnabled() == true)
            {
                m_Shells[i]->Update(aDelta);
            }
        }

        //Update the Barrel
        for (unsigned int i = 0; i < AMOUNT_OF_BARRELS; i++)
        {
            m_Barrel[i]->Update(aDelta);
        }

        m_GreenDetonator->Update(aDelta);
        m_BlueDetonator->Update(aDelta);

    }

    void Game::Draw()
    {
        m_Background->Draw();

        //Draw the shell
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (m_Shells[i]->IsEnabled() == true)
            {
                m_Shells[i]->Draw();
            }
        }

        //Draw Barrel
        for (unsigned int i = 0; i < AMOUNT_OF_BARRELS; i++)
        {
            m_Barrel[i]->Draw();
        }

        //Draw the Detonators
        m_BlueDetonator->Draw();
        m_GreenDetonator->Draw();

        //Draw the Tanks
        m_BlueTank->Draw();
        m_GreenTank->Draw();
        m_ExplodedTank->Draw();

        //Draw the physics world
        Physics::World::GetInstance()->DebugDraw();


        m_Explosion->Draw();
    }

    void Game::HandleKeyDown(KeyCode aKeycode)
    {

    }

    void Game::HandleKeyUp(KeyCode aKeycode)
    {

    }

    void Game::HandleMouseMovement(float aX, float aY)
    {

    }

    void Game::HandleMouseClickDown(MouseButton aMouseButton, float aX, float aY)
    {

    }

    void Game::HandleMouseClickUp(MouseButton aMouseButton, float aX, float aY)
    {

    }

    void Game::HandleResize(float aWidth, float aHeight)
    {
        m_ScreenWidth = aWidth;
        m_ScreenHeight = aHeight;
    }

    void Game::HandleEvent(Event* aEvent)
    {
        //Which type of event was it?
        if (aEvent->GetEventCode() == KEYBOARD_EVENT)
        {
            //Cast the Event object as a MouseMovementEvent
            KeyEvent* keyEvent = (KeyEvent*)aEvent;

            //Handle the keyboard input
            if (keyEvent->GetKeyEventType() == KeyDown)
            {
                HandleKeyDown(keyEvent->GetKeyCode());
            }
            else if (keyEvent->GetKeyEventType() == KeyUp)
            {
                HandleKeyUp(keyEvent->GetKeyCode());
            }
        }
        else if (aEvent->GetEventCode() == MOUSE_MOVEMENT_EVENT)
        {
            //Cast the Event object as a MouseMovementEvent
            MouseMovementEvent* mouseMovementEvent = (MouseMovementEvent*)aEvent;

            //Handle the mouse movement
            HandleMouseMovement(mouseMovementEvent->GetPosition().x, mouseMovementEvent->GetPosition().y);
        }
        else if (aEvent->GetEventCode() == MOUSE_CLICK_EVENT)
        {
            //Cast the Event object as a MouseMovementEvent
            MouseClickEvent* mouseClickEvent = (MouseClickEvent*)aEvent;

            //Handle the mouse click
            if (mouseClickEvent->GetMouseClickEventType() == MouseClickDown)
            {
                HandleMouseClickDown(mouseClickEvent->GetMouseButton(), mouseClickEvent->GetPosition().x, mouseClickEvent->GetPosition().y);
            }
            else if (mouseClickEvent->GetMouseClickEventType() == MouseClickUp)
            {
                HandleMouseClickUp(mouseClickEvent->GetMouseButton(), mouseClickEvent->GetPosition().x, mouseClickEvent->GetPosition().y);
            }
        }
        else if (aEvent->GetEventCode() == RESIZE_EVENT)
        {
            //Cast the Event object as a ResizeEvent
            ResizeEvent* resizeEvent = (ResizeEvent*)aEvent;

            //Handle the resize event
            HandleResize(resizeEvent->GetSize().x, resizeEvent->GetSize().y);
        }
    }

    Camera* Game::GetCamera()
    {
        return m_Camera;
    }

    Shell* Game::GetShell()
    {
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (m_Shells[i]->IsEnabled() == false)
            {
                return m_Shells[i];
            }
        }
        return nullptr;
    }

    void Game::UpdateTank(Tank* aTank, double aDelta)
    {
        aTank->Update(aDelta);

        //Can we fire a shell?
        if (aTank->CanFireShell() == true)
        {
            //Only spawn the shell, if the shell is disabled
            Shell* shell = GetShell();
            if (shell != nullptr)
            {
                GetShell()->Fire(aTank->GetShellPosition(), aTank->GetTurretAngle());
                aTank->ResetFireDelay();
            }
        }
    }

    bool Game::CollisionCallback(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        bool handleCollisionResponse = true;

        Shell* shell = IsShell(aBodyA, aBodyB);
        Barrel* barrel = IsBarrel(aBodyA, aBodyB);
        Tank* tank = IsTank(aBodyA, aBodyB);
        Sprite* detonator = IsDetonator(aBodyA, aBodyB);

        if (tank == m_BlueTank && detonator == m_GreenDetonator)
        {
            m_Camera->Shake(2.5f, 0.4);

            m_GreenDetonator->SetIsEnabled(false);
            m_ExplodedTank->GetPhysicsBody()->SetPosition(Math::PixelsToMeters(m_GreenTank->GetPosition()));
            m_ExplodedTank->GetPhysicsBody()->SetAngle(m_GreenTank->GetPhysicsBody()->GetAngle());
            m_ExplodedTank->SetTurrentAngle((m_GreenTank->getTurret()->GetAngle()));
            m_GreenTank->SetIsEnabled(false);
            m_ExplodedTank->SetIsEnabled(true);
            m_ExplodedTank->SetAnchorPoint(0.2f, 0.5f);
            m_Explosion->SetPosition(Math::MetersToPixels(m_ExplodedTank->GetPhysicsBody()->GetPosition()) + (-(m_GreenTank->GetDirection()) * vec2(m_GreenTank->GetX() / 18, m_GreenTank->GetHeight() / 1.5)));
            m_Explosion->SetIsEnabled(true);
        }

        if (tank == m_GreenTank && detonator == m_BlueDetonator)
        {
            m_Camera->Shake(2.5f, 0.4);



            m_BlueDetonator->SetIsEnabled(false);
            m_ExplodedTank->GetPhysicsBody()->SetPosition(Math::PixelsToMeters(m_BlueTank->GetPosition()));
            m_ExplodedTank->GetPhysicsBody()->SetAngle(m_BlueTank->GetPhysicsBody()->GetAngle());
            m_ExplodedTank->SetTurrentAngle((m_BlueTank->getTurret()->GetAngle()));

            //vec2 direction = m_BlueTank->GetDirection();
            //vec2 worldDirection = m_ExplodedTank->GetWorldDirection();

            //vec2 myDirection = Math::CalculateDirection(m_ExplodedTank->GetAngle());
            //vec2 myWorldDirection = Math::CalculateDirection(m_ExplodedTank->GetWorldAngle());

            //vec2 myCalculatedDirection = vec2(cosf(Math::DegreesToRadians(m_ExplodedTank->GetWorldAngle())), sinf(Math::DegreesToRadians(m_ExplodedTank->GetWorldAngle())));

            m_BlueTank->SetIsEnabled(false);
            m_ExplodedTank->SetIsEnabled(true);
            m_ExplodedTank->SetAnchorPoint(0.5f, 0.5f);
            //float myWidth = -(m_ExplodedTank->GetPhysicsBody()->GetGameObject()->GetWidth()) / 2;
            //float tankWidth = m_ExplodedTank->GetWidth() * (1.0f - m_ExplodedTank->GetAnchorPointX());
            //float tankLength = m_ExplodedTank->GetWidth() * (1.0f - m_ExplodedTank->GetAnchorPointX());
            //vec2 offset = vec2((cosf(m_ExplodedTank->GetAngle()), sinf(m_ExplodedTank->GetAngle())) * vec2(1.0f, m_ExplodedTank->GetHeight() / 2));
            //vec2 offset = (Math::DegreesToRadians(m_ExplodedTank->GetAngle())) * vec2(tankLength, tankLength);
            //vec2 position = Math::MetersToPixels(m_ExplodedTank->GetPhysicsBody()->GetPosition());
            m_Explosion->SetPosition(Math::MetersToPixels(m_ExplodedTank->GetPhysicsBody()->GetPosition()) + (-(m_BlueTank->GetDirection()) * vec2(m_BlueTank->GetX() / 18, m_BlueTank->GetHeight() / 1.5)));

            //m_Explosion->SetPosition(position + vec2(cosf(Math::DegreesToRadians(m_ExplodedTank->GetWorldAngle())) * m_ExplodedTank->GetHeight(), Math::DegreesToRadians(sinf(m_ExplodedTank->GetWorldAngle())) * m_ExplodedTank->GetHeight()));
            m_Explosion->SetIsEnabled(true);
        }

        //If both the Shell and the Barrel pointers aren't null, it means we 
        //have a collision between a Barrel and a Shell
        if (shell != nullptr && barrel != nullptr)
        {

            if (barrel->HasExploded() == false)
            {
                //Explode the barrel
                barrel->Explode();

                //Disable the shell
                shell->SetIsEnabled(false);

                if (barrel->GetPosition() == m_BlueDetonator->GetPosition())
                {
                    barrel->SetIsEnabled(false);
                    m_BlueDetonator->SetIsEnabled(true);
                }
                if (barrel->GetPosition() == m_GreenDetonator->GetPosition())
                {
                    barrel->SetIsEnabled(false);
                    m_GreenDetonator->SetIsEnabled(true);
                }
                m_Camera->Shake(1.0f, 0.2);
            }

            //We don't need to handle collision response when a barrel and a shell collide
            handleCollisionResponse = false;
        }

        if (barrel != nullptr && tank != nullptr)
        {
            if (barrel->IsEnabled() == false)
            {
                handleCollisionResponse = false;
            }
            else
            {
                handleCollisionResponse = true;
            }

            Log("Collision!");
        }

        return handleCollisionResponse; //This will determine if we should handle collision response
    }

    Shell* Game::IsShell(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        for (unsigned int i = 0; i < SHELL_POOL_SIZE; i++)
        {
            if (aBodyA->GetGameObject() == m_Shells[i] || aBodyB->GetGameObject() == m_Shells[i])
            {
                return m_Shells[i];
            }
        }

        //If this return was reached it means neither Physics::Body is a Shell object, return a nullptr
        return nullptr;
    }

    Barrel* Game::IsBarrel(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        for (unsigned int i = 0; i < AMOUNT_OF_BARRELS; i++)
        {
            if (aBodyA->GetGameObject() == m_Barrel[i] || aBodyB->GetGameObject() == m_Barrel[i])
            {
                return m_Barrel[i];
            }
        }

        //If this return was reached it means neither Physics::Body is a Barrel object, return a nullptr
        return nullptr;
    }

    Tank* Game::IsTank(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        if (aBodyA->GetGameObject() == m_GreenTank || aBodyB->GetGameObject() == m_GreenTank)
        {
            return m_GreenTank;
        }

        if (aBodyA->GetGameObject() == m_BlueTank || aBodyB->GetGameObject() == m_BlueTank)
        {
            return m_BlueTank;
        }

        return nullptr;
    }

    Sprite* Game::IsDetonator(Physics::Body* aBodyA, Physics::Body* aBodyB)
    {
        if (aBodyA->GetGameObject() == m_BlueDetonator || aBodyB->GetGameObject() == m_BlueDetonator)
            return m_BlueDetonator;

        if (aBodyA->GetGameObject() == m_GreenDetonator || aBodyB->GetGameObject() == m_GreenDetonator)
            return m_GreenDetonator;

        return nullptr;
    }
}