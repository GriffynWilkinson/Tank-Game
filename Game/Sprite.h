//
//  Sprite.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-05.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Sprite__
#define __GameDev2D__Sprite__

#include "GameObject.h"
#include "../Source/Graphics/Core/Color.h"


namespace GameDev2D
{
    //Forward declarations
    class TextureFrame;
    class Shader;

    //The Sprite class loads a single .png image and allows you to transform (translate, rotate, scale) that image
    class Sprite : public GameObject
    {
    public:
        //Constructs a sprite object with from a .png image file
        Sprite(const string& filename, const string& atlasKey = "");
        virtual ~Sprite();

        //Draws the Sprite
        virtual void Draw();

        //Returns the size of the current frame of the Sprite.
        virtual float GetWidth();
        virtual float GetHeight();

        //Sets the alpha for the Sprite, can be set to animate over a duration, an easing function can be applied, can be revered and repeated
        void SetAlpha(float alpha);

        //Returns the alpha for the Sprite
        float GetAlpha();

        //Changes the image of the Sprite, this will affect the width and height (if different) by not the position, angle, scale, anchorpoint
        virtual void Set(const string& filename, const string& atlasKey = "");

    protected:
        //Protected constructor, so Sprite can be inherited from and not have to actually create a Texture object when the Sprite is created
        Sprite();

        //Member variables
        TextureFrame* m_Texture;
        Color m_Color;
    };
}

#endif /* defined(__GameDev2D__Sprite__) */
