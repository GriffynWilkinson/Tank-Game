    //
//  Audio.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-04-30.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Audio.h"
#include "../Source/Services/AudioManager/AudioManager.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Events/Event.h"


namespace GameDev2D
{
    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* channel, FMOD_CHANNEL_CALLBACKTYPE type, void* commanddata1, void* commanddata2);


    Audio::Audio(const string& aFilename, const string& aExtension, bool aStreamed, bool aLooped) : BaseObject("Audio"),
        m_System(nullptr),
        m_Sound(nullptr),
        m_Channel(nullptr),
        m_Frequency(0),
        m_Volume(0.0f),
        m_Pan(0.0f),
        m_Priority(0),
        m_Position(0),
        m_Length(0),
        m_IsStreamed(aStreamed),
        m_Key("")
    {
        //Load the sound
        m_Sound = ServiceLocator::GetAudioManager()->LoadSound(&m_System, aFilename, aExtension, aStreamed, aLooped);
    
        //Set the audio key
        m_Key = aFilename + "." + aExtension;
    
        //Safety check the sound object
        if(m_Sound != nullptr)
        {
            //Register the audio object with the audio manager
            ServiceLocator::GetAudioManager()->RegisterAudio(this);
        
            //Get the length and default properties of the audio file
            m_Sound->getLength(&m_Length, FMOD_TIMEUNIT_MS);
            m_Sound->getDefaults(&m_Frequency, &m_Volume, &m_Pan, &m_Priority);
            
            //Format the length of the audio string
            stringstream lengthStream;
            if(m_Length > 1000.0f)
            {
                lengthStream << setprecision(4) << static_cast<float>(m_Length) / 1000.0f << " seconds";
            }
            else if(m_Length > 1000.0f * 60.0f)
            {
                lengthStream << setprecision(4) << static_cast<float>(m_Length) / (1000.0f * 60.0f) << " minutes";
            }
            else
            {
                lengthStream << m_Length << " mille-seconds";
            }
        }
    }
    
    Audio::~Audio()
    {
        //Unregister the audio object from the audio manager
        ServiceLocator::GetAudioManager()->UnregisterAudio(this);
    
        //Safety check, then unload the sound
        if(m_Sound != nullptr)
        {
            ServiceLocator::GetAudioManager()->UnloadSound(m_Sound, m_IsStreamed == true ? "" : m_Key);
            m_Sound = nullptr;
        }
        
        //Set the system to null
        m_System = nullptr;
    }
    
    const char* Audio::GetKey()
    {
        return m_Key.c_str();
    }
    
    void Audio::Play()
    {
        //Is the channel null?
        if(m_Channel == nullptr)
        {
            //Safety check the system
            if(m_System != nullptr)
            {
                //Play the sound
                m_System->playSound(FMOD_CHANNEL_FREE, m_Sound, true, &m_Channel);
                
                //Set the channel's user data and call back function
                m_Channel->setUserData(this);
                m_Channel->setCallback(&ChannelCallback);
            }
        }
        
        //Safety check the channel
        if(m_Channel != nullptr)
        {
            bool isPaused = false;
            m_Channel->getPaused(&isPaused);
        
            //Is the audio paused
            if(isPaused == false)
            {
                //Set the channel's frequency, volume, pan, position and priority settings
                m_Channel->setFrequency(m_Frequency);
                m_Channel->setVolume(m_Volume);
                m_Channel->setPan(m_Pan);
                m_Channel->setPriority(m_Priority);
                m_Channel->setPosition(m_Position, FMOD_TIMEUNIT_MS);
            }
            
            //Play the audio
            m_Channel->setPaused(false);
        }
    }
    
    void Audio::Pause()
    {
        //Safety check the channel isn't null
        if(m_Channel != nullptr)
        {
            //Pause the audio channel
            m_Channel->setPaused(true);
        }
    }
    
    void Audio::Stop()
    {
        //Safety check the channel
        if(m_Channel != nullptr)
        {
            //Stop the audio channel
            m_Channel->stop();
        }
    }
    
    bool Audio::IsPlaying()
    {
        //Safety check the channel
        if(m_Channel != nullptr)
        {
            //Determine if the channel playing?
            bool isPlaying = false;
            m_Channel->isPlaying(&isPlaying);
            return isPlaying;
        }
        return false;
    }
    
    void Audio::SetFrequency(float aFrequency)
    {
        if(m_Frequency != aFrequency)
        {
            //Bounds check the frequency, it can't be negative
            m_Frequency = fmaxf(aFrequency, 0.0f);
            
            //Safety check the channel and set the channel's frequency
            if(m_Channel != nullptr)
            {
                m_Channel->setFrequency(m_Frequency);
            }
        }
    }
    
    float Audio::GetFrequency()
    {
        return m_Frequency;
    }
    
    void Audio::SetVolume(float aVolume)
    {
        if(m_Volume != aVolume)
        {
            //Bounds check the volume
            m_Volume = fminf(fmaxf(aVolume, 0.0f), 1.0f);
            
            //Safety check the channel and set the volume
            if(m_Channel != nullptr)
            {
                m_Channel->setVolume(m_Volume);
            }
        }
    }
    
    float Audio::GetVolume()
    {
        return m_Volume;
    }
    
    void Audio::SetPan(float aPan)
    {
        if(m_Pan != aPan)
        {
            //Bounds check the pan
            m_Pan = fminf(fmaxf(aPan, -1.0f), 1.0f);
            
            //Safety check the channel and set the pan
            if(m_Channel != nullptr)
            {
                m_Channel->setPan(m_Pan);
            }
        }
    }
    
    float Audio::GetPan()
    {
        return m_Pan;
    }
    
    void Audio::SetPriority(int aPriority)
    {
        //Set the priority
        m_Priority = aPriority;

        //Safety check the channel and set the priority
        if(m_Channel != nullptr)
        {
            m_Channel->setPriority(m_Priority);
        }
    }
    
    int Audio::GetPriority()
    {
        return m_Priority;
    }
    
    void Audio::SetPosition(unsigned int aPosition)
    {
        //Bounds check the audio's position
        m_Position = aPosition < GetLength() ? aPosition : GetLength();
        
        //Safety check the channel and set the position
        if(m_Channel != nullptr)
        {
            m_Channel->setPosition(aPosition, FMOD_TIMEUNIT_MS);
        }
    }
    
    unsigned int Audio::GetPosition()
    {
        //Safety check the channel and get the position
        if(m_Channel != nullptr)
        {
            m_Channel->getPosition(&m_Position, FMOD_TIMEUNIT_MS);
        }
        return m_Position;
    }
    
    unsigned int Audio::GetRemaining()
    {
        return GetLength() - GetPosition();
    }
        
    unsigned int Audio::GetLength()
    {
        return m_Length;
    }

    void Audio::InvalidateChannel()
    {
        //Set the channel to null
        m_Channel = nullptr;
    }
    
    FMOD_RESULT F_CALLBACK ChannelCallback(FMOD_CHANNEL* aChannel, FMOD_CHANNEL_CALLBACKTYPE aType, void* aCommanddata1, void* aCommanddata2)
    {
        FMOD::Channel* channel = (FMOD::Channel *)aChannel;

        //Playback has reach the end
        if(aType == FMOD_CHANNEL_CALLBACKTYPE_END)
        {
            //Get the user data (audio*) from the channel
            void* userData;
            channel->getUserData(&userData);
            
            //Safety check the user data
            if(userData != nullptr)
            {
                //Dispatch an audio playback finished event and invalidate the channel
                ((Audio*)userData)->InvalidateChannel();
            }
        }

        return FMOD_OK;
    }
}