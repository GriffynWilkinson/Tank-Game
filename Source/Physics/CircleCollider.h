#ifndef __CIRCLE_COLLIDER_H__
#define __CIRCLE_COLLIDER_H__

#include "Collider.h"


namespace GameDev2D
{
    namespace Physics
    {
        class CircleCollider : public Collider
        {
        public:
            CircleCollider(float radius);

            ColliderType GetType();

            float ComputeMass(float density);
            float ComputeInertia(float mass);
            
            float GetRadius();

        private:
            float m_Radius;
        };
    }
}


#endif