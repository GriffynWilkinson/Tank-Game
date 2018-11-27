#ifndef __WORLD_LISTENER_H__
#define __WORLD_LISTENER_H__


namespace GameDev2D
{
    namespace Physics
    {
        //Forward declarations
        class Body;

        class WorldListener
        {
        public:
            virtual ~WorldListener() {}
            virtual bool CollisionCallback(Body* bodyA, Body* bodyB) = 0;
        };
    }
}


#endif