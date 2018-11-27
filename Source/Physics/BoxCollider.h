#ifndef __BOX_COLLIDER_H__
#define __BOX_COLLIDER_H__

#include "Collider.h"


using namespace glm;

namespace GameDev2D
{
    namespace Physics
    {
        //Local constant
        const unsigned int BOX_COLLIDER_VERTEX_COUNT = 4;


        class BoxCollider : public Collider
        {
        public:
            BoxCollider(float width, float height);

            ColliderType GetType();

            float ComputeMass(float density);
            float ComputeInertia(float mass);
            
            float GetWidth();
            float GetHeight();

            vec2 GetVerticesAtIndex(unsigned int index);
            vec2 GetNormalsAtIndex(unsigned int index);

        private:
            //Member variables
            float m_Width;
            float m_Height;
            float m_Angle;
            vec2 m_Vertices[BOX_COLLIDER_VERTEX_COUNT];
            vec2 m_Normals[BOX_COLLIDER_VERTEX_COUNT];
        };
    }
}


#endif