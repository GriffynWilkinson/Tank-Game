 //
//  InputManager.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "InputManager.h"
#include "../ServiceLocator.h"
#include "../../Platforms/PlatformLayer.h"
//#include "../../Graphics/Primitives/Line.h"
#include "../../Events/Input/KeyEvent.h"
#include "../../Events/Input/MouseMovementEvent.h"
#include "../../Events/Input/MouseClickEvent.h"
#include "../../Events/Input/MouseScrollWheelEvent.h"
#include "../../../Game/Game.h"


namespace GameDev2D
{
    InputManager::InputManager() : GameService("InputManager"),
        m_MouseMovementEvent(nullptr)
    {
        //Since accelerometer and gyroscope events are consistent,
        //we can re-use the same event objects instead of creating
        //a new object every n seconds (using 0.01 seconds)
        m_MouseMovementEvent = new MouseMovementEvent();
    }
    
    InputManager::~InputManager()
    {
        //Delete the accelerometer and gryoscope event objects
        SafeDelete(m_MouseMovementEvent);
    }

    void InputManager::Update(double aDelta)
    {
        //Cycle through the key data and update them
        unsigned int i = 0;
        for(i = 0; i < KEY_CODE_COUNT; i++)
        {
            m_Keys[i].Update(aDelta);
        }
        
        //Update the mouse data
        m_MouseData.Update(aDelta);
    }

    void InputManager::Draw()
    {
        
    }
    
    vec2 InputManager::GetMousePosition()
    {
        return m_MouseData.GetPosition();
    }
    
    bool InputManager::IsMouseButtonPressed(MouseButton aMouseButton)
    {
        return m_MouseData.IsPressed(aMouseButton);
    }
    
    bool InputManager::IsKeyPressed(KeyCode aKeyCode)
    {
        return m_Keys[aKeyCode].IsPressed();
    }
    
    void InputManager::HandleKeyDown(KeyCode aKeyCode)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //If the key is pressed, dispatch a KeyRepeat event
        if(m_Keys[aKeyCode].IsPressed() == true)
        {
            //Create the key event
            Event* event = new KeyEvent(KeyRepeat, aKeyCode, m_Keys[aKeyCode].GetDuration());

            //Let the game know about the event
            ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

            //Dispatch the event for anyone else listening
            DispatchEvent(event);
        }
        else
        {
            //Other wise, set the key as pressed
            m_Keys[aKeyCode].SetIsPressed(true);
            
            //Create the key event
            Event* event = new KeyEvent(KeyDown, aKeyCode, 0.0);

            //Let the game know about the event
            ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

            //And dispatch and KeyDown event
            DispatchEvent(event);
        }
    }
    
    void InputManager::HandleKeyUp(KeyCode aKeyCode)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Create the key event
        Event* event = new KeyEvent(KeyUp, aKeyCode, m_Keys[aKeyCode].GetDuration());

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Dispatch a KeyUp event
        DispatchEvent(event);
    
        //And set that the key is no longer pressed
        m_Keys[aKeyCode].SetIsPressed(false);
    }
    
    void InputManager::HandleLeftMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the left mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonLeft, true);
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickDown, MouseButtonLeft, vec2(aX, aY), 0.0);

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Dispatch the mouse click event
        DispatchEvent(event);
    }
    
    void InputManager::HandleLeftMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickUp, MouseButtonLeft, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonLeft));

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Dispatch the mouse click event
        DispatchEvent(event);
        
        //Set that the left mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonLeft, false);
    }
    
    void InputManager::HandleCenterMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the center mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonCenter, true);
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickDown, MouseButtonCenter, vec2(aX, aY), 0.0);

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Lastly, dispatch a MouseClickDown event for the center button
        DispatchEvent(event);
    }
    
    void InputManager::HandleCenterMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickUp, MouseButtonCenter, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonCenter));

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Dispatch a MouseClickUp event for the center button
        DispatchEvent(event);
        
        //Set that the center mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonCenter, false);
    }
    
    void InputManager::HandleRightMouseDown(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Set that the right mouse button is pressed
        m_MouseData.SetIsPressed(MouseButtonRight, true);
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickDown, MouseButtonRight, vec2(aX, aY), 0.0);

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Lastly, dispatch a MouseClickDown event for the right button
        DispatchEvent(event);
    }
    
    void InputManager::HandleRightMouseUp(float aX, float aY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
    
        //Create the mouse click event
        Event* event = new MouseClickEvent(MouseClickUp, MouseButtonRight, vec2(aX, aY), m_MouseData.GetDuration(MouseButtonRight));

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

        //Dispatch a MouseClickUp event for the right button
        DispatchEvent(event);
        
        //Set that the right mouse button is no longer pressed
        m_MouseData.SetIsPressed(MouseButtonRight, false);
    }
    
    void InputManager::HandleMouseMoved(float aX, float aY, float aDeltaX, float aDeltaY)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }
        
        //Set the mouse data's position
        m_MouseData.SetPosition(vec2(aX, aY));
    
        //Do we have a cached mouse movement event object? If we do set the mouse data and dispatch it, 
        //otherwise make a new mouse movement event object and dispatch it
        if(m_MouseMovementEvent != nullptr)
        {
            //Update the mouse movement event
            m_MouseMovementEvent->Set(aX, aY, aDeltaX, aDeltaY);

            //Let the game know about the event
            ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(m_MouseMovementEvent);

            //Dispatch the event
            DispatchEvent(m_MouseMovementEvent, false);
        }
        else
        {
            //Create the mouse click event
            Event* event = new MouseMovementEvent(aX, aY, aDeltaX, aDeltaY);

            //Let the game know about the event
            ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);

            //Dispatch the event
            DispatchEvent(event);
        }
    }
    
    void InputManager::HandleScrollWheel(float aDelta)
    {
        //Ensure the application hasn't been suspended
        if(ServiceLocator::GetPlatformLayer()->IsSuspended() == true)
        {
            return;
        }

        //Create the mouse click event
        Event* event = new MouseScrollWheelEvent(aDelta);

        //Let the game know about the event
        ServiceLocator::GetPlatformLayer()->GetGame()->HandleEvent(event);
    
        //Disptach a scroll wheel event
        DispatchEvent(event);
    }
    
    bool InputManager::CanUpdate()
    {
        return true;
    }
    
    bool InputManager::CanDraw()
    {
        return false;
    }
}