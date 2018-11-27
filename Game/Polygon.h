//
//  Polygon.h
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Polygon__
#define __GameDev2D__Polygon__

#include "../Source/Graphics/Core/Color.h"
#include "../Source/Graphics/Core/RenderMode.h"
#include "../Source/Graphics/Core/Vertex.h"
#include "GameObject.h"

using namespace glm;
using namespace std;


namespace GameDev2D
{
    //Forward declarations
    class Shader;

    //The Polygon class inherits from GameObject. Add vertices to create complex shapes.  
    //A Polygon can be transform (translated, rotatated, scaled). Each vertices position
    //will be relative to the Polygon's position
    class Polygon : public GameObject
    {
    public:
        //Conveniance static methods used to initialize a Polygon in the shape of a Circle
        static void InitializeCircle(Polygon* polygon, float radius, unsigned int lineSegments, bool isFilled, Color color);
        
        //Conveniance static methods used to initialize a Polygon in the shape of a Rect
        static void InitializeRect(Polygon* polygon, float width, float height, bool isFilled, Color color);

        //Constructs the Polygon with a set of vertices
        Polygon(Vertex* vertices, unsigned int count);

        //Constructs the Polygon with zero vertices
        Polygon();
        virtual ~Polygon();
        
        //Draws the polygon
        void Draw();
        
        //Returns the width and height of the polygon
        float GetWidth();
        float GetHeight();

        //Adds a vertex to the polygon, the vertex will be added and set to the active polygon color
        void AddVertex(float x, float y, Color color);
        void AddVertex(vec2 vertex, Color color);
        void AddVertex(vec2 vertex, vec4 color);
        void AddVertex(Vertex vertex);
        
        //Set an array of vertices
        void SetVertices(Vertex* vertices, unsigned int count);

        //Returns a pointer to a vertex for a given index, if the index passed in is out of 
        //bounds, then a null pointer will be returned
        const Vertex* GetVertex(unsigned int index);

        //Returns the number of vertices that make up the Polygon
        unsigned int GetNumberOfVertices();

        //Clears all the vertices
        void ClearVertices();

        //Used to update the vertex buffer object
        void UpdateVertexBuffer();
        
        //Sets the alpha for all the vertices in the Polygon
        void SetAlpha(float alpha);
 
        //Set the point size of the polygon, only applies in the render mode is set to GL_POINTS
        void SetPointSize(float pointSize);
        
        //Returns the point size of the polygon
        float GetPointSize();
        
        //Set the render mode of the polygon
        void SetRenderMode(RenderMode renderMode);

        //Returns the render mode of the polygon
        RenderMode GetRenderMode();

    protected:
        //Used to calculate the size of the Polygon
        void CalculateSize();

        //Member variables
        vector<Vertex> m_Vertices;
        RenderMode m_RenderMode;
        unsigned int m_VertexArrayObject;
        unsigned int m_VertexBufferObject;
        vec2 m_Size;
        float m_PointSize;
        bool m_VertexBufferDirty;
        bool m_ContainsAlpha;
    };
}

#endif /* defined(__GameDev2D__Polygon__) */
