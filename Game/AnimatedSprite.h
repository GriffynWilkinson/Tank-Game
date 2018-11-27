#ifndef __GameDev2D__AnimatedSprite__
#define __GameDev2D__AnimatedSprite__

#include "Sprite.h"


namespace GameDev2D
{
    class AnimatedSprite : public Sprite
    {
    public:
        //Constructs an animated sprite with numerous frames to the animation - more can be added later with the AddFrame method
        AnimatedSprite(const vector<string>& frameFilenames);

        //Constructs an animated sprite without any frames - more can be added later with the AddFrame method
        AnimatedSprite();

        //Destructor
        ~AnimatedSprite();

        //Adds a frame to the back of the animation
        void AddFrame(const string& frameFilename);

        //Adds a frame to the back of the animation
        void AddFrame(const string& filename, const string& atlasKey);

        //Adds the delta time to the animation's elapsed time, 
        //to determine which frame the animation should be on
        void Update(double delta);

        //Returns the size of the current frame of the Sprite.
        float GetWidth();
        float GetHeight();

        //Returns the frame count
        unsigned long GetFrameCount();

        //Returns the frame index
        int GetFrameIndex();

        //Sets the frame index
        void SetFrameIndex(unsigned int frameIndex);

        //Returns the frame speed of the sprite, must be animated
        float GetFrameSpeed();

        //Set the frame speed of an animated sprite
        void SetFrameSpeed(float frameSpeed);

        //Returns wether this Sprite loops, must be animated
        bool DoesLoop();

        //Sets if the Sprite loops, must be animated
        void SetDoesLoop(bool doesLoop);

    private:
        //Member variables
        vector<TextureFrame*> m_Frames;
        unsigned int m_FrameIndex;
        double m_FrameSpeed;
        double m_ElapsedTime;
        bool m_DoesLoop;
    };
}

#endif