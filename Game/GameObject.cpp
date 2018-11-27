#include "GameObject.h"
#include "Polygon.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Utils/Math/Math.h"
#include <gtc/matrix_transform.hpp>


namespace GameDev2D
{
    GameObject::GameObject(const string& aType) : BaseObject(aType),
        m_Parent(nullptr),
        m_Shader(nullptr),
        m_ModelMatrix(mat4(1.0f)),
        m_ResetModelMatrix(false),
        m_AnchorPoint(0.0f, 0.0f),
        m_Position(0.0f, 0.0f),
        m_LinearVelocity(0.0f, 0.0f),
        m_Scale(1.0f, 1.0f),
        m_Angle(0.0f),
        m_AngularVelocity(0.0f),
        m_IsEnabled(true),
        m_DebugRect(nullptr),
        m_DebugCircle(nullptr),
        m_DebugAnchor(nullptr),
        m_AnchorLocation(0.0f, 0.0f)
    {
        //Get the passthrough texture shader
        m_Shader = ServiceLocator::GetShaderManager()->GetPassthroughTextureShader();
    }

    GameObject::~GameObject()
    {
        for(int i = 0; i < m_Children.size(); i++)
        {
            delete m_Children.at(i);
        }
        m_Children.clear();

#if (DRAW_DEBUG_RECT || DRAW_DEBUG_CIRCLE) && DEBUG
        if (m_DebugRect != nullptr)
        {
            delete m_DebugRect;
            m_DebugRect = nullptr;
        }

        if (m_DebugCircle != nullptr)
        {
            delete m_DebugCircle;
            m_DebugCircle = nullptr;
        }

        if (m_DebugAnchor != nullptr)
        {
            delete m_DebugAnchor;
            m_DebugAnchor = nullptr;
        }
#endif
    }

    void GameObject::Update(double aDelta)
    {
        //Only update if the GameObject is enabled
        if (IsEnabled() == true)
        {
            //Calculate the displacement
            vec2 displacement = Math::CalculateDisplacement(m_LinearVelocity, aDelta);

            //Update the position
            SetPosition(GetPosition() + displacement);

            //Calculate the angular displacement
            float angularDisplacement = m_AngularVelocity * aDelta;

            //Set the angle
            SetAngle(GetAngle() + angularDisplacement);

            //Update the children GameObjects
            for (int i = 0; i < m_Children.size(); i++)
            {
                m_Children.at(i)->Update(aDelta);
            }
        }
    }

    void GameObject::Draw()
    {
        //Only Draw if the GameObject is enabled
        if (IsEnabled() == true)
        {
#if DRAW_DEBUG_RECT && DEBUG
            if (m_DebugRect != nullptr)
            {
                m_DebugRect->Draw();
            }
#endif

#if DRAW_DEBUG_CIRCLE && DEBUG
            if (m_DebugCircle != nullptr)
            {
                m_DebugCircle->Draw();
            }
#endif

#if (DRAW_DEBUG_RECT || DRAW_DEBUG_CIRCLE) && DEBUG 
            if (m_DebugAnchor != nullptr)
            {
                m_DebugAnchor->Draw();
            }
#endif

            //Draw the children GameObjects
            for (int i = 0; i < m_Children.size(); i++)
            {
                m_Children.at(i)->Draw();
            }
        }
    }

    vec2 GameObject::GetDirection()
    {
        return Math::CalculateDirection(GetAngle());
    }

    vec2 GameObject::GetWorldDirection()
    {
        return Math::CalculateDirection(GetWorldAngle());
    }

    void GameObject::SetX(float aX)
    {
        if (aX != m_Position.x)
        {
            m_Position.x = aX;
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetY(float aY)
    {
        if (aY != m_Position.y)
        {
            m_Position.y = aY;
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetPosition(float aX, float aY)
    {
        SetX(aX);
        SetY(aY);
    }

    void GameObject::SetPosition(vec2 aPosition)
    {
        SetX(aPosition.x);
        SetY(aPosition.y);
    }

    float GameObject::GetX()
    {
        return m_Position.x;
    }

    float GameObject::GetY()
    {
        return m_Position.y;
    }

    vec2 GameObject::GetPosition()
    {
        return m_Position;
    }

    float GameObject::GetWorldX()
    {
        return GetWorldPosition().x;
    }

    float GameObject::GetWorldY()
    {
        return GetWorldPosition().y;
    }

    vec2 GameObject::GetWorldPosition()
    {
        if (m_Parent != nullptr)
        {
            return m_Parent->GetWorldPosition() + GetPosition();
        }
        return GetPosition();
    }

    void GameObject::SetLinearVelocity(vec2 aLinearVelocity)
    {
        m_LinearVelocity = aLinearVelocity;
    }

    vec2 GameObject::GetLinearVelocity()
    {
        return m_LinearVelocity;
    }

    void GameObject::IncreaseAngle(float aIncrement)
    {
        m_Angle += aIncrement;

        if (m_Angle >= 360.0f)
        {
            m_Angle -= 360.0f;
        }
    }

    void GameObject::DecreaseAngle(float aDecrement)
    {
        m_Angle -= aDecrement;

        if (m_Angle < 0.0f)
        {
            m_Angle += 360.0f;
        }
    }

    void GameObject::SetAngle(float aAngle)
    {
        if (aAngle != m_Angle)
        {
            m_Angle = aAngle;
            ModelMatrixIsDirty();
        }
    }

    float GameObject::GetAngle()
    {
        return m_Angle;
    }

    float GameObject::GetWorldAngle()
    {
        if (m_Parent != nullptr)
        {
            return m_Parent->GetWorldAngle() + GetAngle();
        }
        return GetAngle();
    }

    void GameObject::SetAngularVelocity(float aAngularVelocity)
    {
        m_AngularVelocity = aAngularVelocity;
    }

    float GameObject::GetAngularVelocity()
    {
        return m_AngularVelocity;
    }

    void GameObject::SetScaleX(float aScaleX)
    {
        if (aScaleX != m_Scale.x)
        {
            m_Scale.x = aScaleX;
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetScaleY(float aScaleY)
    {
        if (aScaleY != m_Scale.y)
        {
            m_Scale.y = aScaleY;
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetScale(float aScaleX, float aScaleY)
    {
        SetScaleX(aScaleX);
        SetScaleY(aScaleY);
    }

    void GameObject::SetScale(vec2 aScale)
    {
        SetScaleX(aScale.x);
        SetScaleY(aScale.y);
    }

    float GameObject::GetScaleX()
    {
        return m_Scale.x;
    }

    float GameObject::GetScaleY()
    {
        return m_Scale.y;
    }

    vec2 GameObject::GetScale()
    {
        return m_Scale;
    }

    float GameObject::GetWorldScaleX()
    {
        return GetWorldScale().x;
    }

    float GameObject::GetWorldScaleY()
    {
        return GetWorldScale().y;
    }

    vec2 GameObject::GetWorldScale()
    {
        if (m_Parent != nullptr)
        {
            return m_Parent->GetScale() * GetScale();
        }
        return GetScale();
    }

    void GameObject::SetAnchorX(float aAnchorX)
    {
        if (m_AnchorPoint.x != aAnchorX)
        {
#if CLAMP_ANCHORPOINT
            m_AnchorPoint.x = fminf(fmaxf(aAnchorX, 0.0f), 1.0f);
#else
            m_AnchorPoint.x = aAnchorX;
#endif
            
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetAnchorY(float aAnchorY)
    {
        if (m_AnchorPoint.y != aAnchorY)
        {
#if CLAMP_ANCHORPOINT
            m_AnchorPoint.y = fminf(fmaxf(aAnchorY, 0.0f), 1.0f);
#else
            m_AnchorPoint.y = aAnchorY;
#endif
            ModelMatrixIsDirty();
        }
    }

    void GameObject::SetAnchorPoint(float aAnchorX, float aAnchorY)
    {
        SetAnchorX(aAnchorX);
        SetAnchorY(aAnchorY);
    }

    float GameObject::GetAnchorPointX()
    {
        return m_AnchorPoint.x;
    }

    float GameObject::GetAnchorPointY()
    {
        return m_AnchorPoint.y;
    }

    void GameObject::SetIsEnabled(bool aIsEnabled)
    {
        m_IsEnabled = aIsEnabled;
    }

    bool GameObject::IsEnabled()
    {
        return m_IsEnabled;
    }

    bool GameObject::ContainsPoint(float aX, float aY)
    {
        //Calculate the left and right bounds (this factors in the anchor point)
        float anchorX = GetAnchorPointX();
        float boundsLeft = GetX() - (anchorX * GetWidth());
        float boundsRight = GetX() - (anchorX * GetWidth()) + GetWidth();

        //Calculate the upper and lower bounds (this factors in the anchor point)
        float anchorY = GetAnchorPointY();
        float boundsLower = GetY() - (anchorY * GetHeight());
        float boundsUpper = GetY() - (anchorY * GetHeight()) + GetHeight();

        //If the x and y position with the bounds of the sprite?
        if (aX >= boundsLeft && aX <= boundsRight)
        {
            if (aY >= boundsLower && aY <= boundsUpper)
            {
                return true;
            }
        }

        //If we got here, the x and y position wasn't within the bounds of the sprite
        return false;
    }




    void GameObject::AddChild(GameObject* aChild)
    {
        if (aChild != nullptr)
        {
            //Add the child to the children vector
            m_Children.push_back(aChild);

            //Set the child object's parent
            aChild->m_Parent = this;
        }
    }

    unsigned int GameObject::GetNumberOfChildren()
    {
        return m_Children.size();
    }

    GameObject* GameObject::GetChildAtIndex(unsigned int aIndex)
    {
        if (aIndex < GetNumberOfChildren())
        {
            return m_Children.at(aIndex);
        }
        return nullptr;
    }




    void GameObject::ModelMatrixIsDirty()
    {
        m_ResetModelMatrix = true;
    }

    void GameObject::ResetModelMatrix()
    {
        //Translate the anchor, then translate the position
        mat4 anchor = translate(mat4(1.0f), vec3(-GetWidth() * GetScaleX() * GetAnchorPointX(), -GetHeight() * GetScaleY() * GetAnchorPointY(), 0.0f));
        mat4 viewTranslate = translate(anchor, vec3(GetX(), GetY(), 0.0f));

        //Calculate the rotation based on the anchor point
        mat4 halfTranslate1 = translate(viewTranslate, vec3(GetWidth() * GetScaleX() * GetAnchorPointX(), GetHeight() * GetScaleY() * GetAnchorPointY(), 0.0f));
        
#if (DRAW_DEBUG_RECT || DRAW_DEBUG_CIRCLE) && DEBUG
        vec4 transformedVector = halfTranslate1 * vec4(0.0f, 0.0f, 0.0f, 1.0);
        m_AnchorLocation = vec2(transformedVector.x, transformedVector.y);
#endif
        
        mat4 viewRotation = rotate(halfTranslate1, GetAngle(), vec3(0.0f, 0.0f, 1.0f));
        mat4 halfTranslate2 = translate(viewRotation, vec3(-GetWidth() * GetScaleX() * GetAnchorPointX(), -GetHeight() * GetScaleY() * GetAnchorPointY(), 0.0f));

        //Lastly the scale
        m_ModelMatrix = scale(halfTranslate2, vec3(GetScaleX(), GetScaleY(), 0.0f));


        //NOW we have to factor in the parent's model matrix
        if (m_Parent != nullptr)
        {
            mat4 identity = mat4(1.0f);
            mat4 translation = translate(identity, vec3(m_Parent->GetWorldX(), m_Parent->GetWorldY(), 0.0f));
            mat4 rotation = rotate(translation, m_Parent->GetWorldAngle(), vec3(0.0f, 0.0f, 1.0f));
            mat4 parentModelMatrix = scale(rotation, vec3(m_Parent->GetWorldScaleX(), m_Parent->GetWorldScaleY(), 1.0f));

            m_ModelMatrix = parentModelMatrix * m_ModelMatrix;
        }

        //Clear the model matrix dirty flag
        m_ResetModelMatrix = false;



        //Cycle through and reset the model matrices for all the children
        for(unsigned int i = 0; i < GetNumberOfChildren(); i++)
        {
            m_Children.at(i)->ResetModelMatrix();
        }




#if (DRAW_DEBUG_RECT || DRAW_DEBUG_CIRCLE) && DEBUG 
        //We need to determine if the inheriting class is a polygon,
        //if it is, then we can't draw debug information about it,
        //it would be recurcive. The dynamic cast checks to see if 
        //the inheriting class is a Polygon
        GameObject* gameObject = dynamic_cast<Polygon*>(this);

        //If it isn't create and initialize the debug polygons
        if (gameObject == nullptr)
        {
#if DRAW_DEBUG_RECT && DEBUG
            //If the debug rect hasn't been created yet, create the Polygon object
            if (m_DebugRect == nullptr)
            {
                m_DebugRect = new Polygon();
            }

            //Initialize the rect polygon's size
            Polygon::InitializeRect(m_DebugRect, GetWidth(), GetHeight(), false, DRAW_DEBUG_RECT_COLOR);

            //Update the rest of the rect's properties
            m_DebugRect->SetPosition(GetX(), GetY());
            m_DebugRect->SetAnchorPoint(GetAnchorPointX(), GetAnchorPointY());
            m_DebugRect->SetAngle(GetAngle());
            m_DebugRect->SetScale(GetScaleX(), GetScaleY());
#endif

#if DRAW_DEBUG_CIRCLE && DEBUG
            ////If the debug circle hasn't been created yet, create the Polygon object
            if (m_DebugCircle == nullptr)
            {
                m_DebugCircle = new Polygon();
            }

            //Initialize the circle polygon's size
            float radius = fmaxf(GetWidth(), GetHeight()) / 2.0f;
            Polygon::InitializeCircle(m_DebugCircle, radius, 36, false, DRAW_DEBUG_CIRCLE_COLOR);

            //Update the rest of the circle's properties
            m_DebugCircle->SetPosition(GetX(), GetY());
            m_DebugCircle->SetAnchorPoint(GetAnchorPointX(), GetAnchorPointY());
            m_DebugCircle->SetAngle(GetAngle());
            m_DebugCircle->SetScale(GetScaleX(), GetScaleY());
#endif

            //If the debug polygon hasn't been created yet, create the Polygon object
            if (m_DebugAnchor == nullptr)
            {
                //Initialize all the members that will stay the same
                m_DebugAnchor = new Polygon();
                m_DebugAnchor->AddVertex(vec2(0.0f, DRAW_DEBUG_ANCHORPOINTER_SIZE), DRAW_DEBUG_ANCHORPOINT_COLOR);
                m_DebugAnchor->AddVertex(vec2(0.0f, 0.0f), DRAW_DEBUG_ANCHORPOINT_COLOR);
                m_DebugAnchor->AddVertex(vec2(DRAW_DEBUG_ANCHORPOINTER_SIZE, 0.0f), DRAW_DEBUG_ANCHORPOINT_COLOR);
                m_DebugAnchor->SetRenderMode(RenderMode_LineStrip);
                m_DebugAnchor->UpdateVertexBuffer();
            }

            //Update the rest of the anchor pointer's properties
            m_DebugAnchor->SetPosition(m_AnchorLocation.x, m_AnchorLocation.y);
            m_DebugAnchor->SetAngle(GetAngle());
        }
#endif
    }

    bool GameObject::IsModelMatrixDirty()
    {
        if (m_Parent != nullptr)
        {
            return m_Parent->IsModelMatrixDirty() || m_ResetModelMatrix;
        }
        return m_ResetModelMatrix;
    }
}