//
//  Camera.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-12.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Camera.h"
#include "../../Services/ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
#include "matrix_transform.hpp"
#include "quaternion.hpp"


namespace GameDev2D
{
    Camera::Camera() : BaseObject("Camera"),
        m_ProjectionMatrix(mat4(1.0f)),
        m_ViewMatrix(mat4(1.0f)),
        m_Position(vec2(0.0f, 0.0f)),
        m_Orientation(0.0f),
        m_Zoom(1.0f),
        m_ProjectionMatrixDirty(true),
        m_ViewMatrixDirty(true),
        m_ShakeEnabled(false),
        m_ShakeMagnitude(0.0f),
        m_ShakeDuration(0.0),
        m_ShakeTimer(0.0f),
        m_ShakeOffset(vec2(0.0f, 0.0f))
    {
        //Listener for the resize event, we need to know if the screen resizes to reset the matrices
        ServiceLocator::GetPlatformLayer()->AddEventListener(this, RESIZE_EVENT);
    }
    
    Camera::~Camera()
    {
        //Remove the event listener for the resize event
        ServiceLocator::GetPlatformLayer()->RemoveEventListener(this, RESIZE_EVENT);
    }
    
    void Camera::Update(double aDelta)
    {
        //Is the shake enabled?
        if (m_ShakeEnabled == true)
        {
            //Get the elapsed time
            m_ShakeTimer += aDelta;

            //Has the shake ended?
            if (m_ShakeTimer >= m_ShakeDuration)
            {
                m_ShakeEnabled = false;
                m_ShakeTimer = 0.0;
                m_ShakeDuration = 0.0;
                m_ShakeOffset = vec2(0.0f, 0.0f);
            }
            else
            {
                //Calculate the shake offset based on the progress and magnitude
                float progress = (float)(m_ShakeTimer / m_ShakeDuration);
                float magnitude = (float)(m_ShakeMagnitude * (1.0 - (progress * progress)));
                m_ShakeOffset = vec2(RandomShake(magnitude), RandomShake(magnitude));
            }
            //Set the matrixDirty flag to true
            m_ViewMatrixDirty = true;
        }

        //Should we reset the projection matrix
        if(m_ProjectionMatrixDirty == true)
        {
            ResetProjectionMatrix();
        }
        
        //Should we reset the view matrix
        if(m_ViewMatrixDirty == true)
        {
            ResetViewMatrix();
        }
    }
    
    void Camera::HandleEvent(Event* aEvent)
    {
        //Handle the resize event and reset the projection and view matrices
        switch (aEvent->GetEventCode())
        {
            case RESIZE_EVENT:
                ResetViewMatrix();
                ResetProjectionMatrix();
                break;

            default:
                break;
        }
    }
    
    mat4 Camera::GetProjectionMatrix()
    {
        return m_ProjectionMatrix;
    }
    
    mat4 Camera::GetViewMatrix()
    {
        return m_ViewMatrix;
    }

    float Camera::GetViewWidth()
    {
        return ServiceLocator::GetGraphics()->GetWidth() / m_Zoom;
    }
    
    float Camera::GetViewHeight()
    {
        return ServiceLocator::GetGraphics()->GetHeight() / m_Zoom;
    }
        
    float Camera::GetAspectRatio()
    {
        return GetViewWidth() / GetViewHeight();
    }
    
    vec2 Camera::GetPosition()
    {
        return m_Position;
    }
    
    float Camera::GetOrientation()
    {
        return m_Orientation;
    }
    
    void Camera::SetPositionX(float aPositionX)
    {
        if(m_Position.x != aPositionX)
        {
            m_Position.x = aPositionX;
            m_ViewMatrixDirty = true;
        }
    }
    
    void Camera::SetPositionY(float aPositionY)
    {
        if(m_Position.y != aPositionY)
        {
            m_Position.y = aPositionY;
            m_ViewMatrixDirty = true;
        }
    }
    
    void Camera::SetPosition(float aPositionX, float aPositionY)
    {
        SetPositionX(aPositionX);
        SetPositionY(aPositionY);
    }
    
    void Camera::SetPosition(vec2 aPosition)
    {
        SetPosition(aPosition.x, aPosition.y);
    }
    
    void Camera::SetOrientation(float aOrientation)
    {
        if(m_Orientation != aOrientation)
        {
            m_Orientation = aOrientation;
            m_ViewMatrixDirty = true;
        }
    }
    
    void Camera::SetTransform(vec2 aPosition, float aOrientation)
    {
        SetPosition(aPosition);
        SetOrientation(aOrientation);
    }
    
    float Camera::GetZoom()
    {
        return m_Zoom;
    }
    
    void Camera::SetZoom(float aZoom)
    {
        if(m_Zoom != aZoom)
        {
            m_Zoom = aZoom;
            m_ProjectionMatrixDirty = true;
        }
    }

    void Camera::Shake(float aMagnitude, double aDuration)
    {
        m_ShakeEnabled = true;
        m_ShakeMagnitude = aMagnitude;
        m_ShakeDuration = aDuration;
        m_ShakeTimer = 0.0;
        m_ShakeOffset = vec2(0.0f, 0.0f);
    }
    
    void Camera::ResetProjectionMatrix()
    {
        //Reset the dirty matrix flag
        m_ProjectionMatrixDirty = false;
    
        //Safety check the Graphics service isn't null
        if(ServiceLocator::GetGraphics() != nullptr)
        {
            //Get the view's width and height
            float width = GetViewWidth();
            float height = GetViewHeight();

            //Setup the orthographic projection
            m_ProjectionMatrix = ortho(-width/2.0f, width/2.0f, -height/2.0f, height/2.0f, -1.0f, 1.0f);
        }
        else
        {
            //If the Graphics service is null, return an identity matrix
            m_ProjectionMatrix = mat4(1.0f);
        }
    }
    
    void Camera::ResetViewMatrix()
    {
        //Reset the dirty matrix flag
        m_ViewMatrixDirty = false;
    
        //Safety check the Graphics service isn't null
        if(ServiceLocator::GetGraphics() != nullptr)
        {
            //Get the view width and height, NOT multiplied by the zoom level
            float viewWidth = (float)ServiceLocator::GetGraphics()->GetWidth();
            float viewHeight = (float)ServiceLocator::GetGraphics()->GetHeight();
        
            //Set the view offset
            mat4 viewOffset = translate(mat4(1.0f), vec3(-(viewWidth * 0.5f), -(viewHeight * 0.5f), 0.0f));
            
            //Translate the view
            mat4 viewTranslate = translate(viewOffset, vec3(m_Position.x + m_ShakeOffset.x, m_Position.y + m_ShakeOffset.y, 0.0f));
            
            //Do a half translate so the rotation happens around the center
            mat4 halfTranslate = translate(viewTranslate, vec3((viewWidth / 2.0f), (viewHeight / 2.0f), 0.0f));
            
            //Rotation the view
            mat4 viewRotation = rotate(halfTranslate, m_Orientation, vec3(0.0f, 0.0f, 1.0f));
            
            //Lastly translate back, now that we've rotated around the center
            m_ViewMatrix = translate(viewRotation, vec3(-(viewWidth / 2.0f), -(viewHeight / 2.0f), 0.0f));
        }
        else
        {
            //If the Graphics service is null, return an identity matrix
            m_ViewMatrix = mat4(1.0f);
        }
    }

    float Camera::RandomShake(float aMagnitude)
    {
        float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return (random * 2.0f - 1.0f) * aMagnitude;
    }
}