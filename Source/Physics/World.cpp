#include "World.h"
#include "Body.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "../Utils/Math/Math.h"
#include "../../Game/GameObject.h"
#include "../../Game/Polygon.h"


namespace GameDev2D
{
    namespace Physics
    {
        World* World::s_Instance = nullptr;

        World* World::GetInstance()
        {
            if (s_Instance == nullptr)
            {
                s_Instance = new World();
            }
            return s_Instance;
        }

        World::World() : BaseObject("Physics::World"),
            m_Gravity(0.0f, 0.0f),
            m_Listener(nullptr)
        {

        }

        World::~World()
        {
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                delete m_Bodies.at(i);
            }
            m_Bodies.clear();
        }

        void World::Step(double aTimeStep)
        {
            //Clear the contacts
            m_Contacts.clear();

            //Check Collision
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                for (unsigned int j = i + 1; j < m_Bodies.size(); j++)
                {
                    Body* a = m_Bodies.at(i);
                    Body* b = m_Bodies.at(j);

                    if (a->GetInverseMass() == 0.0f || b->GetInverseMass() == 0.0f)
                    {
                        continue;
                    }

                    //Initilaize a Manifold object
                    Manifold manifold(a, b);

                    //Check the collision
                    if (CheckCollision(a, b, &manifold) == true)
                    {
                        //There was a collision notify the listener
                        if (m_Listener != nullptr)
                        {
                            //If the collisionCallback method returns true, then
                            //add the manifold to the contacts vector
                            if (m_Listener->CollisionCallback(a, b) == true)
                            {
                                m_Contacts.push_back(manifold);
                            }
                        }
                        else
                        {
                            //If there is no listener set then we assume we add all
                            //contacts
                            m_Contacts.push_back(manifold);
                        }

                    }
                }
            }

            //
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                m_Bodies.at(i)->SyncForces(aTimeStep, m_Gravity);
            }

            //Correct overlap before the sync velocities
            for (unsigned int i = 0; i < m_Contacts.size(); i++)
            {
                m_Contacts.at(i).CorrectOverlap();
            }

            //
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                m_Bodies.at(i)->SyncVelocities(aTimeStep);
            }

            //
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                m_Bodies.at(i)->ClearForces();
            }

            //
            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                //Get the GameObject attached to the physics body
                GameObject* gameObject = m_Bodies.at(i)->GetGameObject();

                //Safety check
                if (gameObject != nullptr)
                {
                    gameObject->SetPosition(Math::MetersToPixels(m_Bodies.at(i)->GetPosition()));
                    gameObject->SetAngle(Math::RadiansToDegrees(m_Bodies.at(i)->GetAngle()));
                }
            }
        }

        void World::DebugDraw()
        {
#if DEBUG && DRAW_DEBUG_PHYSICS_WORLD
            Polygon p;

            for (unsigned int i = 0; i < m_Bodies.size(); i++)
            {
                Body* body = m_Bodies.at(i);
                Collider* collider = body->GetCollider();

                if (collider->GetType() == ColliderType_Circle)
                {
                    CircleCollider* circle = (CircleCollider*)collider;
                    float radius = Math::MetersToPixels(circle->GetRadius());
                    vec2 position = Math::MetersToPixels(body->GetPosition());
                    float angle = Math::RadiansToDegrees(body->GetAngle());

                    Polygon::InitializeCircle(&p, radius, 36, false, Color::YellowColor());
                    p.SetPosition(position);
                    p.SetAngle(angle);
                    p.SetAnchorPoint(0.5f, 0.5f);
                    p.Draw();
                }
                else if (collider->GetType() == ColliderType_Box)
                {
                    BoxCollider* box = (BoxCollider*)collider;
                    float width = Math::MetersToPixels(box->GetWidth());
                    float height = Math::MetersToPixels(box->GetHeight());
                    vec2 position = Math::MetersToPixels(body->GetPosition());
                    float angle = Math::RadiansToDegrees(body->GetAngle());

                    Polygon::InitializeRect(&p, width, height, false, Color::YellowColor());
                    p.SetPosition(position);
                    p.SetAngle(angle);
                    p.SetAnchorPoint(0.5f, 0.5f);
                    p.Draw();
                }
            }
#endif
        }

        Body* World::CreateBody(Collider* aCollider, float aDensity)
        {
            Body* body = new Body(aCollider, aDensity);
            m_Bodies.push_back(body);

            return body;
        }

        void World::SetGravity(vec2 aGravity)
        {
            m_Gravity = aGravity;
        }

        vec2 World::GetGravity()
        {
            return m_Gravity;
        }

        void World::SetListener(WorldListener* aListener)
        {
            m_Listener = aListener;
        }

        bool World::CheckCollision(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
        {
            bool result = false;

            //Determine the type of collision check to perform
            if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
                aBodyB->GetCollider()->GetType() == ColliderType_Circle)
            {
                result = CheckCircleToCircle(aBodyA, aBodyB, aManifold);
            }

            else if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
                aBodyB->GetCollider()->GetType() == ColliderType_Box)
            {
                result = CheckCircleToBox(aBodyA, aBodyB, aManifold);
            }

            else if (aBodyA->GetCollider()->GetType() == ColliderType_Box &&
                aBodyB->GetCollider()->GetType() == ColliderType_Circle)
            {
                result = CheckCircleToBox(aBodyB, aBodyA, aManifold);

                //because we flipped the bodies, we need to flip the normal (A to B)
                aManifold->FlipNormal();
            }

            return result;
        }

        bool World::CheckCircleToCircle(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
        {
            if (aBodyA->GetGameObject() != nullptr && aBodyA->GetGameObject()->IsEnabled() == false)
            {
                return false;
            }

            if (aBodyB->GetGameObject() != nullptr && aBodyB->GetGameObject()->IsEnabled() == false)
            {
                return false;
            }

            //Get the Circle Colliders
            CircleCollider* circleColliderA = (CircleCollider*)aBodyA->GetCollider();
            CircleCollider* circleColliderB = (CircleCollider*)aBodyB->GetCollider();

            //Check the distance against the radii for collision
            float distanceSquared = Math::CalculateDistanceSquared(aBodyA->GetPosition(), aBodyB->GetPosition());
            float combinedRadii = circleColliderA->GetRadius() + circleColliderB->GetRadius();
            float radiiSquared = combinedRadii * combinedRadii;

            if (distanceSquared > radiiSquared)
            {
                return false;
            }

            //Calculate the overlap and the collision normal
            float distance = sqrtf(distanceSquared);
            float overlap = combinedRadii - distance;
            vec2 normal = vec2(aBodyB->GetPosition() - aBodyA->GetPosition()) / distance;
            aManifold->SetContact(overlap, normal);

            return true;
        }

        bool World::CheckCircleToBox(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
        {
            if (aBodyA->GetGameObject() != nullptr && aBodyA->GetGameObject()->IsEnabled() == false)
            {
                return false;
            }
            if (aBodyB->GetGameObject() != nullptr && aBodyB->GetGameObject()->IsEnabled() == false)
            {
                return false;
            }

            CircleCollider* circleCollider = (CircleCollider*)aBodyA->GetCollider();
            BoxCollider* boxCollider = (BoxCollider*)aBodyB->GetCollider();

            float radius = circleCollider->GetRadius();
            float radiusSquared = radius * radius;
            float angle = boxCollider->GetAngle();
            float c = cosf(angle);
            float s = sinf(angle);
            mat2 orientation = mat2(c, -s, s, c);

            vec2 circleCenter = glm::transpose(orientation) * (aBodyA->GetPosition() - aBodyB->GetPosition());

            float maxSeperation = -FLT_MAX;
            unsigned int faceNormal = 0;

            for (unsigned int i = 0; i < BOX_COLLIDER_VERTEX_COUNT; i++)
            {
                vec2 vertex1 = boxCollider->GetNormalsAtIndex(i);
                vec2 vertex2 = circleCenter - boxCollider->GetVerticesAtIndex(i);
                float seperation = Math::Dot(vertex1, vertex2);

                if (seperation > radius)
                {
                    return false;
                }

                if (seperation > maxSeperation)
                {
                    maxSeperation = seperation;
                    faceNormal = i;
                }
            }

            if (maxSeperation < EPSILON)
            {
                //Calculate the overlap and collision normal
                float overlap = circleCollider->GetRadius();
                vec2 normal = -(orientation * boxCollider->GetNormalsAtIndex(faceNormal));
                aManifold->SetContact(overlap, normal);

                return true;
            }

            vec2 vertex1 = boxCollider->GetVerticesAtIndex(faceNormal);
            unsigned int index2 = (faceNormal + 1 < BOX_COLLIDER_VERTEX_COUNT) ? faceNormal + 1 : 0;
            vec2 vertex2 = boxCollider->GetVerticesAtIndex(index2);

            vec2 closestPoint = Math::CalculateClosestPoint(circleCenter, radius, vertex1, vertex2);

            float distance = Math::CalculateDistanceSquared(closestPoint, circleCenter);

            if (distance <= radiusSquared)
            {
                //Calculate the overlap and the collision normal
                float overlap = radius - distance;
                vec2 normal = circleCenter - closestPoint;
                normal = -(orientation * normal);
                normal = Math::Normalize(normal);
                aManifold->SetContact(overlap, normal);

                return true;
            }

            return false;

        }
    }
}