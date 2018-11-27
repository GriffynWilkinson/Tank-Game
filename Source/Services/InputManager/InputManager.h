//
//  InputManager.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-20.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__InputManager__
#define __GameDev2D__InputManager__

#include "../GameService.h"
#include "../../Input/KeyData.h"
#include "../../Input/MouseData.h"
#include "../../Input/KeyCodes.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class MouseMovementEvent;

    //The InputManager game service handles all input for all platforms, including mouse and keyboard
    // and dispatches events for the input it receieves.
    //This class dispatches the following events:
    // KEYBOARD_EVENT
    // MOUSE_MOVEMENT_EVENT
    // MOUSE_CLICK_EVENT
    // MOUSE_WHEEL_EVENT
    class InputManager : public GameService
    {
    public:
        InputManager();
        ~InputManager();
        
        //Used to Update and Draw the InputManager
        void Update(double delta);
        void Draw();

        //Returns the mouse position in the window
        vec2 GetMousePosition();

        //Returns wether the mouse button is pressed or not
        bool IsMouseButtonPressed(MouseButton mouseButton);
        
        //Returns wether the key is pressed or not
        bool IsKeyPressed(KeyCode keyCode);
        
        //Methods to handle Keyboard input
        void HandleKeyDown(KeyCode keyCode);
        void HandleKeyUp(KeyCode keyCode);
        
        //Methods to handle Mouse input
        void HandleLeftMouseDown(float x, float y);
        void HandleLeftMouseUp(float x, float y);
        void HandleCenterMouseDown(float x, float y);
        void HandleCenterMouseUp(float x, float y);
        void HandleRightMouseDown(float x, float y);
        void HandleRightMouseUp(float x, float y);
        void HandleMouseMoved(float x, float y, float deltaX, float deltaY);
        void HandleScrollWheel(float delta);
        
        //Methods used to determine if this GameService should be drawn and updated
        bool CanUpdate();
        bool CanDraw();
    
    private:
        //Keyboard data
        KeyData m_Keys[KEY_CODE_COUNT];
        
        //Mouse data
        MouseData m_MouseData;

        //Event object that are cached because they are thrown very 
        //frequently, this avoids having to call 'new' many times a second
        MouseMovementEvent* m_MouseMovementEvent;
    };
}

#endif /* defined(__GameDev2D__InputManager__) */
