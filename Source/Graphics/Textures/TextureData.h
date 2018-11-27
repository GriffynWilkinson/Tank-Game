#ifndef __GameDev2D__TextureData__
#define __GameDev2D__TextureData__

#include "TextureFormat.h"

namespace GameDev2D
{
    struct TextureData
    {
        TextureData(TextureFormat format, unsigned int width, unsigned int height)
        {
            this->format = format;
            this->width = width;
            this->height = height;
            this->data = nullptr;
        }

        TextureData(TextureFormat format, unsigned int width, unsigned int height, unsigned char* data)
        {
            this->format = format;
            this->width = width;
            this->height = height;
            this->data = data;
        }

        //Member variables
        TextureFormat format;
        unsigned int width;
        unsigned int height;
        unsigned char* data;
    };
}

#endif