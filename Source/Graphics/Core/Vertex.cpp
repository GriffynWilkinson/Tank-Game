#include "Vertex.h"


namespace GameDev2D
{
    Vertex::Vertex(float aX, float aY, Color aColor) : BaseObject("Vertex")
    {
        SetPosition(aX, aY);
        SetColor(aColor);
    }

    Vertex::Vertex(vec2 aPosition, Color aColor) : BaseObject("Vertex")
    {
        SetPosition(aPosition);
        SetColor(aColor);
    }

    Vertex::Vertex(vec2 aPosition, vec4 aColor) : BaseObject("Vertex")
    {
        SetPosition(aPosition);
        SetColor(aColor);
    }

    Vertex::Vertex() : BaseObject("Vertex")
    {
        SetPosition(vec2(0.0f, 0.0f));
        SetColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void Vertex::SetPosition(float aX, float aY)
    {
        m_Vertex[0] = aX;
        m_Vertex[1] = aY;
    }

    void Vertex::SetPosition(vec2 aPosition)
    {
        SetPosition(aPosition.x, aPosition.y);
    }

    void Vertex::SetColor(float aRed, float aGreen, float aBlue, float aAlpha)
    {
        m_Vertex[2] = aRed;
        m_Vertex[3] = aGreen;
        m_Vertex[4] = aBlue;
        m_Vertex[5] = aAlpha;
    }

    void Vertex::SetColor(vec4 aColor)
    {
        SetColor(aColor.r, aColor.g, aColor.b, aColor.a);
    }

    void Vertex::SetColor(Color aColor)
    {
        SetColor(aColor.Red(), aColor.Green(), aColor.Blue(), aColor.Alpha());
    }

    vec2 Vertex::GetPosition()
    {
        return vec2(m_Vertex[0], m_Vertex[1]);
    }

    vec4 Vertex::GetColor()
    {
        return vec4(m_Vertex[2], m_Vertex[3], m_Vertex[4], m_Vertex[5]);
    }

    float* Vertex::GetArray()
    {
        return m_Vertex;
    }
}