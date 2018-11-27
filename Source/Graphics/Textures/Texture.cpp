//
//  Texture.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-01.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Texture.h"
#include "../../Services/ServiceLocator.h"
#include "../../Services/Graphics/Graphics.h"


namespace GameDev2D
{
    Texture::Texture(TextureData* aTextureData, unsigned int aMipmapLevel) : BaseObject("Texture"),
        m_TextureData(aTextureData),
        m_Id(0)
    {
        //Generate an id for the Texture
        ServiceLocator::GetGraphics()->GenerateTexture(&m_Id);

        //Pass the TextureData onto the graphics card
        ServiceLocator::GetGraphics()->SetTextureData(m_Id, m_TextureData, aMipmapLevel);

        //Delete the image data, we are done with it
        SafeDeleteArray(m_TextureData->data);
    }
    
    Texture::~Texture()
    {
        //Delete the texture from the GPU
        if(m_Id != 0)
        {
            ServiceLocator::GetGraphics()->DeleteTexture(&m_Id);
            m_Id = 0;
        }

        //Delete the texture data object
        if (m_TextureData != nullptr)
        {
            delete m_TextureData;
            m_TextureData = nullptr;
        }
    }
    
    unsigned int Texture::GetWidth()
    {
        return m_TextureData->width;
    }
    
    unsigned int Texture::GetHeight()
    {
        return m_TextureData->height;
    }

    TextureFormat Texture::GetFormat()
    {
        return m_TextureData->format;
    }
    
    unsigned int Texture::GetId()
    {
        return m_Id;
    }
}
