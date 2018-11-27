#ifndef __BARREL_H__
#define __BARREL_H__

#include "GameObject.h"


namespace GameDev2D
{
    //Forward declaration
    class Sprite;
    class AnimatedSprite;
    namespace Physics
    {
        class Body;
    }

    class Barrel : public GameObject
    {
    public:
        Barrel();
        ~Barrel();

        //Sets the position of the sprite
        void SetX(float x);
        void SetY(float y);
        void SetPosition(float x, float y);
        void SetPosition(vec2 position);

        float GetWidth();
        float GetHeight();

        void Explode();
        bool HasExploded();

    private:
        //Member variable
        Sprite* m_Barrel;
        AnimatedSprite* m_Explosion;
        Physics::Body* m_PhysicsBody;
    };
}

#endif