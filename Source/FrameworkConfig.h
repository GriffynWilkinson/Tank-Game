//
//  Config.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-13.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef GameDev2D_Config_h
#define GameDev2D_Config_h

//Windows Platform settings
#define WINDOWS_TARGET_FPS 60
#define WINDOWS_WINDOW_TITLE "GameDev2D - Windows"
#define WINDOWS_DEFAULT_WIDTH 1200
#define WINDOWS_DEFAULT_HEIGHT 800
#define WINDOWS_DEFAULT_FULLSCREEN false
#define WINDOWS_DEFAULT_VERTICAL_SYNC false
#define WINDOWS_DEFAULT_OPENGL_VERSION_MAJOR 3 //Don't change
#define WINDOWS_DEFAULT_OPENGL_VERSION_MINOR 3 //Don't change

//OSX Platform settings
#define OSX_TARGET_FPS 60
#define OSX_WINDOW_TITLE "GameDev2D - OSX"
#define OSX_DEFAULT_WIDTH 1024
#define OSX_DEFAULT_HEIGHT 768
#define OSX_DEFAULT_FULLSCREEN false
#define OSX_DEFAULT_VERTICAL_SYNC false

//General settings
#define CLAMP_ANCHORPOINT 0

//Graphics settings
#define GRAPHICS_DEFAULT_CLEAR_COLOR Color::CornflowerBlueColor()

//Debug Draw
#define DRAW_DEBUG_FPS_COUNTER 1
#define DRAW_DEBUG_DELTA_TIME 0
#define DRAW_DEBUG_ELAPSED_TIME 0
#define DRAW_DEBUG_RECT 0
#define DRAW_DEBUG_CIRCLE 0
#define DRAW_DEBUG_PHYSICS_WORLD 0

#define DEBUG_UI_FONT_COLOR Color::WhiteColor()
#define DRAW_DEBUG_RECT_COLOR Color::RedColor()
#define DRAW_DEBUG_CIRCLE_COLOR Color::GreenColor()
#define DRAW_DEBUG_ANCHORPOINT_COLOR Color::BlueColor()

#define DRAW_DEBUG_ANCHORPOINTER_SIZE 15.0f

//Errors
#define THROW_EXCEPTION_ON_ERROR 1

//Memory (Don't use)
#define PURGE_UNUSED_TEXTURES_IMMEDIATELY 0
#define PURGE_UNUSED_SHADERS_IMMEDIATELY 0
#define PURGE_UNUSED_FONTS_IMMEDIATELY 0

//Logging
#define LOG_VERBOSITY_MASK VerbosityLevel_Debug | VerbosityLevel_Events
#define LOG_TO_FILE 0
#define LOG_FILE "/Log.txt"
#define LOG_ERRORS_TO_FILE 0
#define LOG_ERROR_FILE "/Errors.txt"

#endif
