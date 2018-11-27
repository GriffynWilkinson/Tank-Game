#include "AnimatedSprite.h"
#include "../Source/Graphics/Textures/TextureFrame.h"


namespace GameDev2D
{
    AnimatedSprite::AnimatedSprite(const vector<string>& aFrameFilenames) : Sprite(),
        m_FrameIndex(0),
        m_FrameSpeed(15.0),
        m_ElapsedTime(0.0),
        m_DoesLoop(false)
    {
        for (unsigned int i = 0; i < aFrameFilenames.size(); i++)
        {
            AddFrame(aFrameFilenames.at(i));
        }
    }

    AnimatedSprite::AnimatedSprite() : Sprite(),
        m_FrameIndex(0),
        m_FrameSpeed(15.0),
        m_ElapsedTime(0.0),
        m_DoesLoop(false)
    {

    }

    AnimatedSprite::~AnimatedSprite()
    {
        //Set the texture to null so that the Sprite doesn't try to delete it, 
        //the texture object will be deleted in the for loop below
        m_Texture = nullptr;

        //Cycle through and delete all the frames in the AnimatedSprite
        for (unsigned int i = 0; i < m_Frames.size(); i++)
        {
            delete m_Frames.at(i);
            m_Frames.at(i) = nullptr;
        }

        m_Frames.clear();
    }

    void AnimatedSprite::AddFrame(const string& aFrameFilename)
    {
        //Create the texture frame object
        TextureFrame* texture = new TextureFrame(aFrameFilename, "");
        texture->SetShader(m_Shader);
        texture->SetColor(m_Color);

        //Add a texture frame to the frames vector
        m_Frames.push_back(texture);
    }

    void AnimatedSprite::AddFrame(const string& aFilename, const string& aAtlasKey)
    {
        //Create the texture frame object
        TextureFrame* texture = new TextureFrame(aFilename, aAtlasKey);
        texture->SetShader(m_Shader);
        texture->SetColor(m_Color);

        //Add a texture frame to the frames vector
        m_Frames.push_back(texture);
    }

    void AnimatedSprite::Update(double aDelta)
    {
        if (IsEnabled() == true)
        {
            //Increment the elapsed time.
            m_ElapsedTime += aDelta;

            //Calculate the new frame index.
            int lastFrameIndex = m_FrameIndex;
            int frameIndex = (int)(m_ElapsedTime / (1.0 / m_FrameSpeed));

            //If the animation loops, we need to see if it is ready wrap around to the start.
            if (m_DoesLoop == true)
            {
                frameIndex = frameIndex % GetFrameCount();
            }

            //If the frame index has changed, set the new frame index.
            if (frameIndex != lastFrameIndex)
            {
                SetFrameIndex(frameIndex);
            }
        }
    }

    float AnimatedSprite::GetWidth()
    {
        if (GetFrameCount() > 0)
        {
            return m_Frames.at(m_FrameIndex)->GetSourceFrame().size.x;
        }
        return 0.0f;
    }

    float AnimatedSprite::GetHeight()
    {
        if (GetFrameCount() > 0)
        {
            return m_Frames.at(m_FrameIndex)->GetSourceFrame().size.y;
        }
        return 0.0f;
    }

    unsigned long AnimatedSprite::GetFrameCount()
    {
        return m_Frames.size();
    }

    int AnimatedSprite::GetFrameIndex()
    {
        return m_FrameIndex;
    }

    void AnimatedSprite::SetFrameIndex(unsigned int aFrameIndex)
    {
        //Safety check the frame index.
        if (aFrameIndex < GetFrameCount())
        {
            //Set the frames index.
            m_FrameIndex = aFrameIndex;

            //Set the elapsed time
            m_ElapsedTime = (double)m_FrameIndex * (1.0 / m_FrameSpeed);

            //Flag the model matrix as dirty
            ModelMatrixIsDirty();

            //Set the Sprite's texture to the current frame
            m_Texture = m_Frames.at(m_FrameIndex);
        }
    }

    float AnimatedSprite::GetFrameSpeed()
    {
        return m_FrameSpeed;
    }

    void AnimatedSprite::SetFrameSpeed(float aFrameSpeed)
    {
        m_FrameSpeed = aFrameSpeed;
    }

    bool AnimatedSprite::DoesLoop()
    {
        return m_DoesLoop;
    }

    void AnimatedSprite::SetDoesLoop(bool aDoesLoop)
    {
        m_DoesLoop = aDoesLoop;
    }
}