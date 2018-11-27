#ifndef __MANIFOLD_H__
#define __MANIFOLD_H__


using namespace glm;

namespace GameDev2D
{
    namespace Physics
    {
        //Local constants
        const float OVERLAP_ALLOWANCE = 0.05f;
        const float OVERLAP_PCT_TO_CORRECT = 0.6f; //Percentage to correct

        //Forward declaration
        class Body;

        class Manifold
        {
        public:
            Manifold(Body* bodyA, Body* bodyB);

            void SetContact(float overlap, vec2 normal);

            void FlipNormal();

            void CorrectOverlap();

        private:
            //Member variables
            Body* m_BodyA;
            Body* m_BodyB;
            float m_Overlap;      // Amount of overlap from collision
            vec2 m_Normal;        // From A to B
        };
    }
}

#endif