#include "Manifold.h"
#include "Body.h"
#include "Collider.h"
#include "../Utils/Math/Math.h"


namespace GameDev2D
{
    namespace Physics
    {
        Manifold::Manifold(Body* aBodyA, Body* aBodyB) :
            m_BodyA(aBodyA),
            m_BodyB(aBodyB),
            m_Overlap(0.0f),
            m_Normal(0.0f, 0.0f)
        {

        }

        void Manifold::CorrectOverlap()
        {
            vec2 correction = (std::max(m_Overlap - OVERLAP_ALLOWANCE, 0.0f) / (m_BodyA->GetInverseMass() + m_BodyB->GetInverseMass())) * m_Normal * OVERLAP_PCT_TO_CORRECT;
            m_BodyA->SetPosition(m_BodyA->GetPosition() - correction * m_BodyA->GetInverseMass());
            m_BodyB->SetPosition(m_BodyB->GetPosition() + correction * m_BodyB->GetInverseMass());
        }

        void Manifold::FlipNormal()
        {
            m_Normal = -m_Normal;
        }

        void Manifold::SetContact(float aOverlap, vec2 aNormal)
        {
            m_Overlap = aOverlap;
            m_Normal = aNormal;
        }
    }
}
