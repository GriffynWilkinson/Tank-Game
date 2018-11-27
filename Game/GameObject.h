#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "FrameworkConfig.h"
#include "../Source/Core/BaseObject.h"
#include "glm.hpp"

using namespace glm;

namespace GameDev2D
{
    //Forward declarations
    class Shader;
    class Polygon;

    //GameObject class definition - abstract class
    class GameObject : public BaseObject
    {
    public:
        GameObject(const string& type);
        virtual ~GameObject();

        //Default implementation of Update for each GameObject to 
        //properly update their internal logic
        virtual void Update(double delta);

        //Any class that inherits from GameObject MUST
        //have a unique implementation of Draw()
        virtual void Draw();

        //Pure virtual method
        //Any class that inherits from GameObject MUST
        //have a unique implementation of GetWidth() and GetHeight()
        virtual float GetWidth() = 0;
        virtual float GetHeight() = 0;

        //Returns the current direction of the GameObject
        vec2 GetDirection();

        //Returns the current direction, relative to the parent's direction
        vec2 GetWorldDirection();

        //Sets the position of the sprite
        virtual void SetX(float x);
        virtual void SetY(float y);
        virtual void SetPosition(float x, float y);
        virtual void SetPosition(vec2 position);

        //Returns the position of the sprite
        float GetX();
        float GetY();
        vec2 GetPosition();

        //The world position, is relative to the parent's position
        float GetWorldX();
        float GetWorldY();
        vec2 GetWorldPosition();

        //Sets the linear velocity
        void SetLinearVelocity(vec2 linearVelocity);

        //Returns the linear velocity of the GameObject
        vec2 GetLinearVelocity();

        //Increases the local angle (in degrees) by the desired amount
        void IncreaseAngle(float increment);

        //Decreases the local angle (in degrees) by the desired amount
        void DecreaseAngle(float decrement);

        //Sets the angle of the GameObject
        void SetAngle(float angle);

        //Returns the  angle of the GameObject
        float GetAngle();

        //The world's angle, is relative to the parent's angle
        float GetWorldAngle();

        //Sets the angular velocity of the GameObject
        void SetAngularVelocity(float angularVelocity);

        //Returns the angular velocity of the GameObject
        float GetAngularVelocity();

        //Sets the scale of the Sprite
        void SetScaleX(float scaleX);
        void SetScaleY(float scaleY);
        void SetScale(float scaleX, float scaleY);
        void SetScale(vec2 scale);

        //Returns the scale of the Sprite
        float GetScaleX();
        float GetScaleY();
        vec2 GetScale();

        //Returns the scale, relative to the parent's scale
        float GetWorldScaleX();
        float GetWorldScaleY();
        vec2 GetWorldScale();

        //Sets the anchor point for the Sprite, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAnchorX(float anchorX);
        void SetAnchorY(float anchorY);
        void SetAnchorPoint(float anchorX, float anchorY);

        //Returns the anchor point of the Sprite
        float GetAnchorPointX();
        float GetAnchorPointY();

        //Sets wether the GameObject is enabled or not
        void SetIsEnabled(bool isEnabled);

        //Returns wether the GameObhect is enabled or not
        bool IsEnabled();

        //Returns wether the x and y position is inside the Sprite
        bool ContainsPoint(float x, float y);

        //Adds a child to the GameObject
        void AddChild(GameObject* child);

        //Returns the number of children
        unsigned int GetNumberOfChildren();

        //Returns the GameObject for the given index
        GameObject* GetChildAtIndex(unsigned int index);

        //This function indicates that the model matrix is dirty, there is an option to reset the
        //model matrix immediately. If the matrix is NOT reset immediately it will be done after
        //the Sprite has been updated.
        void ModelMatrixIsDirty();

    protected:
        //Used to Reset the Model Matrix
        void ResetModelMatrix();

        //Returns wether the model matrix is dirty
        bool IsModelMatrixDirty();

        //Member variables

        GameObject* m_Parent;
        vector<GameObject*> m_Children;

        Shader* m_Shader;
        mat4 m_ModelMatrix;
        bool m_ResetModelMatrix;
        vec2 m_AnchorPoint;
        vec2 m_Position;
        vec2 m_LinearVelocity;
        vec2 m_Scale;
        float m_Angle;
        float m_AngularVelocity;
        bool m_IsEnabled;
        Polygon* m_DebugRect;
        Polygon* m_DebugCircle;
        Polygon* m_DebugAnchor;
        vec2 m_AnchorLocation;
    };
}

#endif