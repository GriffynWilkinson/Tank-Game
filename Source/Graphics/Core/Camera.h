//
//  Camera.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Camera__
#define __GameDev2D__Camera__

#include "../../Core/BaseObject.h"
#include "../../Events/EventHandler.h"


using namespace glm;

namespace GameDev2D
{
    //2D Orthographic camera, used by every scene in the game. A scene can have more than one camera,
    //but currently the Graphics service only supports one active camera at a time. The camera positions,
    //orientation, and zoom can be set to animate over a duration, an easing function can be applied,
    //can be revered and repeated. A camera shake can also be applied to the camera view.
    class Camera : public BaseObject, public EventHandler
    {
    public:
        Camera();
        ~Camera();
        
        //Update method
        void Update(double delta);
        
        //EventHandler method
        void HandleEvent(Event* event);
        
        //Getter methods for the projection and view matrices
        mat4 GetProjectionMatrix();
        mat4 GetViewMatrix();
        
        //Gets the view width and height, factors in the current zoom level
        float GetViewWidth();
        float GetViewHeight();
        
        //Get the aspect ratio
        float GetAspectRatio();
        
        //Getter methods for the camera position and orientation
        vec2 GetPosition();
        float GetOrientation();
        
        //Sets the position of the camera
        void SetPositionX(float positionX);
        void SetPositionY(float positionY);
        void SetPosition(float positionX, float positionY);
        void SetPosition(vec2 position);
        
        //Sets the orientation of the camera
        void SetOrientation(float orientation);
        
        //Sets the transform (position and orientation of the camera
        void SetTransform(vec2 position, float orientation);
        
        //Returns the current zoom level of the camera
        float GetZoom();
        
        //Sets the zoom level of the camera
        void SetZoom(float zoom);

        //Shakes the camera for a magnitude over a certain duration
        void Shake(float magnitude, double duration);

        //Resets the projection and view matrices
        void ResetProjectionMatrix();
        void ResetViewMatrix();
        
    protected:
        //Conveniance method to randomize a camera shake
        float RandomShake(float magnitude);

        //Member variables
        mat4 m_ProjectionMatrix;
        mat4 m_ViewMatrix;
        vec2 m_Position;
        float m_Orientation;
        float m_Zoom;
        bool m_ProjectionMatrixDirty;
        bool m_ViewMatrixDirty;

        //Shake members
        bool m_ShakeEnabled;
        float m_ShakeMagnitude;
        double m_ShakeDuration;
        double m_ShakeTimer;
        vec2 m_ShakeOffset;
    };
}

#endif /* defined(__GameDev2D__Camera__) */
