//
//  Texture.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-01.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Texture__
#define __GameDev2D__Texture__

#include "../../Core/BaseObject.h"
#include "TextureData.h"
#include "../OpenGL.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //TODO: Better mipmap implementation

    //The Texture class is responsible for loading PNG texture data in OpenGL. It can
    //also be used to load image data manually or generate an empty texture.
    class Texture : public BaseObject
    {
    public:
        //Creates a Texture object using the texture data
        Texture(TextureData* textureData, unsigned int mipmapLevel = 0);

        //Destructor
        virtual ~Texture();

        //Returns the width of the texture
        unsigned int GetWidth();

        //Returns the height of the texture
        unsigned int GetHeight();
        
        //Returns the format of the texture
        TextureFormat GetFormat();
        
        //Returns the id of the texture
        unsigned int GetId();

    private:
        //Member variables
        TextureData* m_TextureData;
        unsigned int m_Id;
    };
}

#endif /* defined(__GameDev2D__Texture__) */
