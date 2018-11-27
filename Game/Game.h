//
//  Game.h
//  GameDev2D
//
//  Created by Bradley Flood on 12-08-30.
//  Copyright (c) 2012 Algonquin College. All rights reserved.
//

#ifndef GAME_H
#define GAME_H

#include "../Source/Input/Input.h"
#include "../Source/Physics/WorldListener.h"


using namespace glm;

namespace GameDev2D
{
    //Local constants
    const unsigned int SHELL_POOL_SIZE = 6;
    const unsigned int AMOUNT_OF_BARRELS = 48;

    //Forward declarations
    class Camera;
    class Event;
    class Sprite;
    class AnimatedSprite;
    class Label;
    class Audio;
    class Polygon;
    class Tank;
    class Shell;
    class Barrel;

    //Forward declare physics classes
    namespace Physics
    {
        class Body;
        class World;
    }

    class Game : public BaseObject, public Physics::WorldListener
    {
    public:
        Game();
        ~Game();

        //Update game content in this method
        void Update(double delta);

        //Draw game content in this method
        void Draw();

        //Convenience methods to handle key up and key down events
        void HandleKeyDown(KeyCode keycode);
        void HandleKeyUp(KeyCode keycode);

        //Convenience methods to handle mouse events
        void HandleMouseMovement(float x, float y);
        void HandleMouseClickDown(MouseButton mouseButton, float x, float y);
        void HandleMouseClickUp(MouseButton mouseButton, float x, float y);

        //Convenience method to handle a screen resize event
        void HandleResize(float width, float height); //*NEW*

        //Message pump used to handle events passed to the game (input, resize, fullscreen, etc)
        void HandleEvent(Event* event);

        //Returns a pointer to the Game's camera object
        Camera* GetCamera();

        //Returns a disabled Shell object from the Shell object pool
        Shell* GetShell();

        //Updates and handles input for a Tank object
        void UpdateTank(Tank* aTank, double aDelta);

        //Handle collision between bodies
        bool CollisionCallback(Physics::Body* bodyA, Physics::Body* bodyB);

        //Conveniance method to determine if a Phyiscs::Body is a Shell object
        //Method will return nullptr if neither Phyiscs::Body is a Shell
        Shell* IsShell(Physics::Body* bodyA, Physics::Body* bodyB);

        //Conveniance method to determine if a Phyiscs::Body is a Barrel object
        //Method will return nullptr if neither Phyiscs::Body is a Barrel
        Barrel* IsBarrel(Physics::Body* bodyA, Physics::Body* bodyB);

        Tank* IsTank(Physics::Body* bodyA, Physics::Body* bodyB);

		Sprite* IsDetonator(Physics::Body* bodyA, Physics::Body* bodyB);

    private:
        //Member variables
        Camera* m_Camera;
        Tank* m_BlueTank;
        Tank* m_GreenTank;
		Tank* m_ExplodedTank;
        Shell* m_Shells[SHELL_POOL_SIZE];
        Barrel* m_Barrel[AMOUNT_OF_BARRELS];
        Sprite* m_Background;
		Sprite* m_BlueDetonator;
		Sprite* m_GreenDetonator;
        AnimatedSprite* m_Explosion;
        double m_TimeStep;
        float m_ScreenWidth;
        float m_ScreenHeight;
    };
}

#endif
