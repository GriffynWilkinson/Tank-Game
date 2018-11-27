#include "CircleCollider.h"


namespace GameDev2D
{
    namespace Physics
    {
        CircleCollider::CircleCollider(float aRadius) : Collider(),
            m_Radius(aRadius)
        {

        }

        ColliderType CircleCollider::GetType()
        {
            return ColliderType_Circle;
        }

        float CircleCollider::ComputeMass(float aDensity)
        {
            return M_PI * m_Radius * m_Radius * aDensity;
        }

        float CircleCollider::ComputeInertia(float aMass)
        {
            return aMass * m_Radius * m_Radius;
        }

        float CircleCollider::GetRadius()
        {
            return m_Radius;
        }
    }
}