#ifndef GameDev2D_PngLoader_h
#define GameDev2D_PngLoader_h


namespace GameDev2D
{
    class TextureData;

    class PngLoader
    {
    public:
        static bool LoadFromPath(const std::string& path, TextureData** textureData);
    };
}

#endif