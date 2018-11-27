#ifndef GameDev2D_RenderMode_h
#define GameDev2D_RenderMode_h

namespace GameDev2D
{
    enum RenderMode
    {
        RenderMode_Points = 0,
        RenderMode_Lines,
        RenderMode_LineLoop,
        RenderMode_LineStrip,
        RenderMode_Triangles,
        RenderMode_TriangleStrip,
        RenderMode_TriangleFan,
        RenderMode_Quads,
        RenderMode_QuadStrip,
        RenderMode_Polygon
    };
}

#endif