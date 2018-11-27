#ifndef __SHELL_H__
#define __SHELL_H__

#include "GameObject.h"

//Shell constants
const float SHELL_SPEED = 1000.0f;
const float SHELL_FORCE = 100.0f;


namespace GameDev2D
{
    //Forward declaration
    class Sprite;
    namespace Physics
    {
        class Body;
    }

    class Shell : public GameObject
    {
    public:
        Shell();
        ~Shell();

        void Update(double delta);

        float GetWidth();
        float GetHeight();

        void Fire(vec2 position, float angleInDegrees);

    private:
        //Member variables
        Sprite* m_Sprite;
        Physics::Body* m_PhysicsBody;
    };
}

#endif