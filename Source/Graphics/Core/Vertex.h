#ifndef __GameDev2D__Vertex__
#define __GameDev2D__Vertex__

#include "../../Core/BaseObject.h"
#include "Color.h"


namespace GameDev2D
{
    class Vertex : public BaseObject
    {
    public:
        Vertex(float x, float y, Color color);
        Vertex(vec2 position, Color color);
        Vertex(vec2 position, vec4 color);
        Vertex();

        void SetPosition(float x, float y);
        void SetPosition(vec2 position);

        void SetColor(float red, float green, float blue, float alpha);
        void SetColor(vec4 color);
        void SetColor(Color color);

        vec2 GetPosition();
        vec4 GetColor();

        float* GetArray();

    private:
        float m_Vertex[6];
    };
}

#endif