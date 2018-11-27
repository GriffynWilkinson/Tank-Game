//
//  PlatformLayer.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-10.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//


#include "PlatformLayer.h"
#include "../Services/ServiceLocator.h"
#include "../../Game/Game.h"
#if __APPLE__
#include <unistd.h>
#endif


namespace GameDev2D
{
    PlatformLayer::PlatformLayer(const string& aPlatformType, int aFrameRate) : BaseObject(aPlatformType), EventDispatcher(),
        m_Game(nullptr),
        m_Width(0),
        m_Height(0),
        m_IsSuspended(false),
        m_IsRunning(true),
        m_Frames(0),
        m_FramesPerSecond(0),
        m_RefreshRate(0),
        m_DeltaTicks(0),
        m_FrameRate(0),
        m_FrameTicks(0),
		m_LastFrameTicks(0),
		m_LastFPSTicks(0)
    {
        //Seed the random number generator
        srand((unsigned int)time(nullptr));

        //Set the target framerate
        SetTargetFramerate(aFrameRate);
        
        //Set ServiceLocator's PlatformLayer pointer
        ServiceLocator::SetPlatformLayer(this);
    }
    
    PlatformLayer::~PlatformLayer()
    {    
        //Remove all the game services
        ServiceLocator::RemoveAllServices();
    }

    void PlatformLayer::Init()
    {
        //Initialize the log files
        InitLogFiles();
        
        //Log the platform details (CPU, RAM, GPU and hardware info)
        LogPlatformDetails();
        
        //Initialize the Game services
        ServiceLocator::LoadDefaultServices();

        //Initialize the starting ticks value
        m_StartTicks = GetTicks();
    }
    
    void PlatformLayer::InitLogFiles()
    {
        #if LOG_TO_FILE
        {
            //Get the Path for the Working Directory and append the Log filename to it
            string path = string(GetWorkingDirectory());
            path += LOG_FILE;
                    
            //Open the log file, wiping any existing data in the file
            FILE* logFile;
            logFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if(logFile != NULL)
            {
                fprintf(logFile, "[%s] - Allocated at: %p\n", GetType().c_str(), this);
                fclose(logFile);
            }
        }
        #endif
        
        #if LOG_ERRORS_TO_FILE
        {
            //Get the Path for the Working Directory and append the Error log filename to it
            string path = string(GetWorkingDirectory());
            path += LOG_ERROR_FILE;
                    
            //Open the error log file, wiping any existing data in the file
            FILE* errorFile;
            errorFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if(errorFile != NULL)
            {
                fclose(errorFile);
            }
        }
        #endif
    }
    
    void PlatformLayer::LogPlatformDetails()
    {
        //Local variables
        string name;
        string model;
        string version;

        //Get the platform name, model and operating system version
        PlatformName(name);
        PlatformModel(model);
        PlatformVersion(version);

        //Log platform information
        Log(VerbosityLevel_Debug, "Platform: %s %s", name.c_str(), version.c_str());
        if(model != "") 
        {
            Log(VerbosityLevel_Debug, "Model: %s", model.c_str());
        }
        Log(VerbosityLevel_Debug, "CPU cores: %u", CpuCount());
        Log(VerbosityLevel_Debug, "Memory installed: %.2f-MB", ((float)MemoryInstalled() / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space total: %.2f-GB", ((float)DiskSpaceTotal() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space used: %.2f-GB", ((float)DiskSpaceUsed() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "Disk space free: %.2f-GB", ((float)DiskSpaceFree() / 1024.0f / 1024.0f / 1024.0f));
        Log(VerbosityLevel_Debug, "GPU: %s", (const char*)glGetString(GL_RENDERER));
        Log(VerbosityLevel_Debug, "OpenGL API: %s", (const char*)glGetString(GL_VERSION));
    }

    bool PlatformLayer::GameLoop()
    {
        if(m_IsSuspended == false)
        {
            //Update the frame
            bool isRunning = Update();
            
            //Draw the frame
            Draw();
            
            //Return wether the game loop still running
            return isRunning;
        }
        
        return m_IsRunning;
    }
    
    void PlatformLayer::Resume()
    {
        //Set the isSuspended flag to false
        m_IsSuspended = false;

        //Reset the last frame ticks, to avoid a large delta time after the app suspension
        m_LastFrameTicks = GetTicks();
        
        //Dispatch a Resume Event
        DispatchEvent(new Event(RESUME_EVENT));
    }
    
    void PlatformLayer::Suspend()
    {
        //Set the isSuspended flag to true
        m_IsSuspended = true;
        
        //Dispatch a Suspend Event
        DispatchEvent(new Event(SUSPEND_EVENT));
    }
    
    void PlatformLayer::Shutdown()
    {
        if(m_IsRunning == true)
        {
            //Set the isRunning flag to false
            m_IsRunning = false;
            
            //Dispatch a Shutdown Event
            DispatchEvent(new Event(SHUTDOWN_EVENT));
        }
    }

    Game* PlatformLayer::GetGame()
    {
        return m_Game;
    }
    
    int PlatformLayer::GetWidth()
    {
        return m_Width;
    }
    
    int PlatformLayer::GetHeight()
    {
        return m_Height;
    }
    
    float PlatformLayer::GetScale()
    {
        return 1.0f;
    }
    
    void PlatformLayer::ResizeTo(unsigned int aWidth, unsigned int aHeight)
    {
        //Does nothing, each platform will have to override to implement resizing
    }
    
    unsigned int PlatformLayer::GetDeltaTicks()
    {
        return m_DeltaTicks;
    }
    
    double PlatformLayer::GetDeltaTime()
    {
        if (m_Frames == 0)
        {
            return 1.0 / (double)m_FrameRate;
        }

        return (double)(m_DeltaTicks / 1000.0);
    }
    
    double PlatformLayer::GetElapsedTime()
    {
        double elapsed = (double)(m_FrameTicks - m_StartTicks) / 1000.0;
        return elapsed;
    }
    
    unsigned int PlatformLayer::GetFramesPerSecond()
    {
        return m_FramesPerSecond;
    }
    
    void PlatformLayer::SetTargetFramerate(unsigned int aFramerate)
    {
        m_FrameRate = aFramerate;
        m_RefreshRate = 1000.0 / (double)m_FrameRate;
    }

    unsigned int PlatformLayer::GetTargetFramerate()
    {
        return m_FrameRate;
    }

    bool PlatformLayer::IsFullscreen()
    {
        return false;
    }

    void PlatformLayer::SetIsFullscreen(bool aIsFullscreen)
    {
        //Does nothing, each platform will have to implement to support fullscreen, that is if the platform supports fullscreen
    }
    
    bool PlatformLayer::IsSuspended()
    {
        return m_IsSuspended;
    }
    
    void PlatformLayer::SetRenderBufferStorage()
    {
        //By default this method does nothing, platforms that require
        //a custom render buffer storage can override this method
    }

    bool AreSame(double a, double b)
    {
        return fabs(a - b) < 0.1;
    }

    bool PlatformLayer::CanTick()
    {
        //Sleep the main thread to achieve our target framerate
        m_FrameTicks = GetTicks();
        m_DeltaTicks = m_FrameTicks - m_LastFrameTicks;

        //If the refresh rate - the delta ticks is greater than 0.1, then skip the frame
        if (m_RefreshRate - m_DeltaTicks > 0.1)
        {
            return false;
        }

        //Track the last frame tick
        m_LastFrameTicks = m_FrameTicks;

        //We can process this frame
        return true;
    }
    
    void PlatformLayer::Tick()
    {
        //Clamp the delta ticks
		if(m_DeltaTicks > 1000)
        {
			m_DeltaTicks = 1000;
        }
        
        //Get the delta time
        double deltaTime = GetDeltaTime();

        //Update the Game's services
        ServiceLocator::UpdateServices(deltaTime);

        //Update the Game and its camera
        if (m_Game != nullptr)
        {
            m_Game->Update(deltaTime);

            if (m_Game->GetCamera() != nullptr)
            {
                m_Game->GetCamera()->Update(deltaTime);
            }
        }

        //Increment the frames
        m_Frames++;

        //Calculate the frames per second
        if (m_FrameTicks - m_LastFPSTicks >= 1000)
        {
            m_FramesPerSecond = m_Frames;
            m_Frames = 0;
            m_LastFPSTicks = m_FrameTicks;
        }
    }
}