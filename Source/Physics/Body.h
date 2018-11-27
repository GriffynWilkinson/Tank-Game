#ifndef __BODY_H__
#define __BODY_H__


using namespace glm;

namespace GameDev2D
{
    //Forward declaration
    class GameObject;

    namespace Physics
    {
        //Forward declaration
        class Collider;

        class Body
        {
        public:
            Body(Collider* collider, float density);
            ~Body();

            void SetPosition(vec2 position);
            vec2 GetPosition();

            void SetAngle(float angleInRadians);
            float GetAngle();

            void SetLinearVelocity(vec2 linearVelocity);
            vec2 GetLinearVelocity();

            void SetAngularVelocity(float angularVelocity);
            float GetAngularVelocity();

            void SetLinearDamping(vec2 linearDamping);
            vec2 GetLinearDamping();

            void SetAngularDamping(float angularDamping);
            float GetAngularDamping();

            void SetMass(float mass);
            float GetMass();
            float GetInverseMass();

            void SetInertia(float inertia);
            float GetInertia();
            float GetInverseInertia();

            void ApplyForce(vec2 force);
            void ApplyTorque(float torque);

            void ApplyLinearImpulse(vec2 linearImpulse);
            void ApplyAngularImpulse(float angularImpulse);

            void ClearForces();

            void SyncForces(double timeStep, vec2 gravity);
            void SyncVelocities(double timeStep);

            void SetGameObject(GameObject* gameObject);
            GameObject* GetGameObject();

            Collider* GetCollider();

        private:
            //Member variables
            vec2 m_Position;    //In meters
            float m_Angle;      //In radians

            vec2 m_LinearVelocity;   //In m/s
            float m_AngularVelocity; //In radians/second

            vec2 m_Force;   //In Newtons
            float m_Torque; //Angular force

            vec2 m_LinearDamping;
            float m_AngularDamping;

            float m_Mass;       //In kg
            float m_InverseMass;
            float m_Inertia;    //In kg-square meters
            float m_InverseInertia;

            GameObject* m_GameObject;
            Collider* m_Collider;
        };
    }
}

#endif