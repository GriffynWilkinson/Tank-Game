#include "PngLoader.h"
#include "../../Graphics/Textures/TextureData.h"
#include <png.h>


namespace GameDev2D
{
    bool PngLoader::LoadFromPath(const std::string& aPath, TextureData** aTextureData)
    {
        //Local variables
        png_structp png_ptr;
        png_infop info_ptr;
        unsigned int sig_read = 0;
        FILE* file = nullptr;

        //Open the image file
        if ((file = fopen(aPath.c_str(), "rb")) == NULL)
        {
            return false;
        }

        //Create and initialize the png_struct with the desired error handler
        //functions.  If you want to use the default stderr and longjump method,
        //you can supply NULL for the last three parameters.  We also supply the
        //the compiler header file version, so that we know if the application
        //was compiled with a compatible version of the library.  REQUIRED
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == nullptr)
        {
            fclose(file);
            return false;
        }

        //Allocate/initialize the memory for image information.  REQUIRED.
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == nullptr)
        {
            //Free all of the memory associated with the png_ptr
            png_destroy_read_struct(&png_ptr, NULL, NULL);

            //Close the open file
            fclose(file);

            //If we get here, we had a problem reading the file
            return false;
        }

        //Set error handling if you are using the setjmp/longjmp method (this is
        //the normal method of doing things with libpng).  REQUIRED unless you
        //set up your own error handlers in the png_create_read_struct() earlier.
        if (setjmp(png_jmpbuf(png_ptr)))
        {
            //Free all of the memory associated with the png_ptr and info_ptr
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

            //Close the open file
            fclose(file);

            //If we get here, we had a problem reading the file
            return false;
        }

        //Set up the input control if you are using standard C streams
        png_init_io(png_ptr, file);

        //If we have already read some of the signature
        png_set_sig_bytes(png_ptr, sig_read);

        //If you have enough memory to read in the entire image at once,
        //and you need to specify only transforms that can be controlled
        //with one of the PNG_TRANSFORM_* bits (this presently excludes
        //quantizing, filling, setting background, and doing gamma
        //adjustment), then you can read the entire image (including
        //pixels) into the info structure with this call:
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

        //Get the data, width and height of the image.
        png_size_t rowBytes = png_get_rowbytes(png_ptr, info_ptr);
        png_bytepp rowPointers = png_get_rows(png_ptr, info_ptr);
        unsigned int width = png_get_image_width(png_ptr, info_ptr);
        unsigned int height = png_get_image_height(png_ptr, info_ptr);
        
        //Safety check that there is actually image data
        if (width > 0 && height > 0)
        {
            //Set the texture color format
            unsigned char colorType = png_get_color_type(png_ptr, info_ptr);
            TextureFormat format = colorType == PNG_COLOR_TYPE_RGB ? TextureFormat_RGB : TextureFormat_RGBA;

            //Allocate the image data
            unsigned char* data = new unsigned char[rowBytes*height];

            //The png image is ordered top to bottom, but OpenGL expect it bottom to top so the order or swapped
            for (unsigned int i = 0; i < height; i++)
            {
                memcpy(data + (rowBytes * (height - 1 - i)), rowPointers[i], rowBytes);
            }

            //Set the texture data
            *aTextureData = new TextureData(format, width, height, data);
        }

        //Clean up after the read, and free any memory allocated - REQUIRED
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        //Close the file
        fclose(file);

        //If we got here, then everthing was successful
        return true;
    }
}