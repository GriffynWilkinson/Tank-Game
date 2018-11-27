#ifndef __WORLD_H__
#define __WORLD_H__

#include "../Core/BaseObject.h"
#include "WorldListener.h"
#include "Manifold.h"


using namespace glm;

namespace GameDev2D
{
    //Forward declarations
    class GameObject;

    namespace Physics
    {
        //Forward declarations
        class Body;
        class Collider;

        class World : public BaseObject
        {
        public:
            static World* GetInstance();

            void Step(double timeStep);

            void DebugDraw();

            Body* CreateBody(Collider* collider, float density);

            void SetGravity(vec2 gravity);
            vec2 GetGravity();

            void SetListener(WorldListener* listener);

        private:
            //Private to ensure Singleton design pattern
            World();
            ~World();

            //Private collision methods
            bool CheckCollision(Body* bodyA, Body* bodyB, Manifold* manifold);
            bool CheckCircleToCircle(Body* bodyA, Body* bodyB, Manifold* manifold);
            bool CheckCircleToBox(Body* bodyA, Body* bodyB, Manifold* manifold);

            //Member variables
            vec2 m_Gravity;
            vector<Body*> m_Bodies;

            vector<Manifold> m_Contacts;

            WorldListener* m_Listener;
            static World* s_Instance;
        };
    }
}

#endif