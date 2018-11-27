//
//  Polygon.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-18.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Polygon.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Services/Graphics/Graphics.h"
#include "../Source/Services/ShaderManager/ShaderManager.h"


namespace GameDev2D
{ 
    void Polygon::InitializeCircle(Polygon* aPolygon, float aRadius, unsigned int aLineSegments, bool aIsFilled, Color aColor)
    {
        //Safety check the radius
        if (aRadius == 0.0f)
        {
            return;
        }

        //Clear the vertices
        aPolygon->ClearVertices();

        //Calculate the rotation amount
        float rotationAmount = (360.0f / (float)aLineSegments);

        //Set the vertices
        vec2 vertex;
        for (float i = 0; i < 359.99f; i += rotationAmount)
        {
            vertex.x = aRadius - (cosf(((float)M_PI * i / 180.0f)) * aRadius);
            vertex.y = aRadius - (sinf(((float)M_PI * i / 180.0f)) * aRadius);
            aPolygon->AddVertex(vertex, aColor);
        }

        //Set the render mode
        aPolygon->SetRenderMode(aIsFilled ? RenderMode_TriangleFan : RenderMode_LineLoop);

        //Update the vertex buffer
        aPolygon->UpdateVertexBuffer();
    }

    void Polygon::InitializeRect(Polygon* aPolygon, float aWidth, float aHeight, bool aIsFilled, Color aColor)
    {
        //Safety check the radius
        if (aWidth == 0.0f && aHeight == 0.0f)
        {
            return;
        }

        //Clear the vertices
        aPolygon->ClearVertices();

        //Add the vertices
        aPolygon->AddVertex(vec2(0.0f, aHeight), aColor);
        aPolygon->AddVertex(vec2(aWidth, aHeight), aColor);
        aPolygon->AddVertex(vec2(aWidth, 0.0f), aColor);
        aPolygon->AddVertex(vec2(0.0f, 0.0f), aColor);

        //Set the render mode
        aPolygon->SetRenderMode(aIsFilled ? RenderMode_TriangleFan : RenderMode_LineLoop);

        //Update the vertex buffer
        aPolygon->UpdateVertexBuffer();
    }

    Polygon::Polygon(Vertex* aVertices, unsigned int aCount) : GameObject("Polygon"),
        m_RenderMode(RenderMode_TriangleFan),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0),
        m_Size(0.0f, 0.0f),
        m_PointSize(1.0f),
        m_VertexBufferDirty(false),
        m_ContainsAlpha(false)
    {
        //Set the shader as the default passthrough shader
        m_Shader = ServiceLocator::GetShaderManager()->GetPassthroughShader();
        
        //Set the vertices
        SetVertices(aVertices, aCount);
        
        //Intialize the position and angle
        SetPosition(0.0f, 0.0f);
        SetAngle(0.0f);
        
        //Set the model matrix to dirty
        ModelMatrixIsDirty();
    }

    Polygon::Polygon() : GameObject("Polygon"),
        m_RenderMode(RenderMode_TriangleFan),
        m_VertexArrayObject(0),
        m_VertexBufferObject(0),
        m_Size(0.0f, 0.0f),
        m_PointSize(1.0f),
        m_VertexBufferDirty(false),
        m_ContainsAlpha(false)
    {
        //Set the shader as the default passthrough shader
        m_Shader = ServiceLocator::GetShaderManager()->GetPassthroughShader();
    }
    
    Polygon::~Polygon()
    {
        //Set the shader to null
        m_Shader = nullptr;
        
        //Delete the VBO
        if(m_VertexBufferObject != 0)
        {
            glDeleteBuffers(1, &m_VertexBufferObject);
            m_VertexBufferObject = 0;
        }

        //Delete the VAO
        if(m_VertexArrayObject != 0)
        {
            glDeleteVertexArrays(1, &m_VertexArrayObject);
            m_VertexArrayObject = 0;
        }
        
        //Clear the vertices vector
        m_Vertices.clear();
    }
    
    void Polygon::Draw()
    {
        if (IsEnabled() == true)
        {
            //Safety check the shader
            if (m_Shader == nullptr)
            {
                return;
            }

            //If the model matrix is dirty, reset it
            if (IsModelMatrixDirty() == true)
            {
                ResetModelMatrix();
            }

            //Update the vertex buffer if it hasn't already been updated
            if (m_VertexBufferDirty == true)
            {
                UpdateVertexBuffer();
            }

            //Use the shader
            m_Shader->Use();

            //Set the point size attribute
            int pointSizeIndex = m_Shader->GetAttribute("a_pointSize");
            glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
            glVertexAttrib1f(pointSizeIndex, m_PointSize);

            //Cache the graphics service
            Graphics* graphics = ServiceLocator::GetGraphics();

            //Bind the vertex array object
            graphics->BindVertexArray(m_VertexArrayObject);

            //Set the model view projection matrix
            mat4 mvp = graphics->GetProjectionMatrix() * graphics->GetViewMatrix() * m_ModelMatrix;
            glUniformMatrix4fv(m_Shader->GetModelViewProjectionUniform(), 1, 0, &mvp[0][0]);

            //Validate the shader
            if (m_Shader->Validate() == false)
            {
                return;
            }

            //Disable blending, if we did in fact have it enabled
            if (m_ContainsAlpha == true)
            {
                graphics->EnableBlending();
            }

            //Render the polygon
            glDrawArrays(m_RenderMode, 0, (GLsizei)m_Vertices.size());

            //Disable blending, if we did in fact have it enabled
            if (m_ContainsAlpha == true)
            {
                graphics->DisableBlending();
            }

            //Unbind the vertex array
            ServiceLocator::GetGraphics()->BindVertexArray(0);
        }
    }

    void Polygon::AddVertex(float aX, float aY, Color aColor)
    {
        AddVertex(Vertex(aX, aY, aColor));
    }

    void Polygon::AddVertex(vec2 aVertex, Color aColor)
    {
        AddVertex(Vertex(aVertex, aColor));
    }

    void Polygon::AddVertex(vec2 aVertex, vec4 aColor)
    {
        AddVertex(Vertex(aVertex, aColor));
    }
        
    void Polygon::AddVertex(Vertex aVertex)
    {
        //Add the vertex to the vertices vector
        m_Vertices.push_back(Vertex(aVertex.GetPosition(), aVertex.GetColor()));

        //The vertex buffer is now dirty
        m_VertexBufferDirty = true;
    }

    void Polygon::SetVertices(Vertex* aVertices, unsigned int aCount)
    {
        //Clear all the vertices
        ClearVertices();
        
        //Cycle through and add all the vertices
        for(unsigned int i = 0; i < aCount; i++)
        {
            AddVertex(aVertices[i]);
        }
        
        //Update the vertex buffers
        UpdateVertexBuffer();
    }

    const Vertex* Polygon::GetVertex(unsigned int aIndex)
    {
        if (aIndex < GetNumberOfVertices())
        {
            return &m_Vertices.at(aIndex);
        }
        return nullptr;
    }

    unsigned int Polygon::GetNumberOfVertices()
    {
        return m_Vertices.size();
    }

    void Polygon::ClearVertices()
    {
        //Clear the vertices vector
        m_Vertices.clear();

        //The vertex buffer is now dirty
        m_VertexBufferDirty = true;
    }

    float Polygon::GetWidth()
    {
        return m_Size.x;
    }

    float Polygon::GetHeight()
    {
        return m_Size.y;
    }
    
    void Polygon::SetAlpha(float aAlpha)
    {
        //Cycle through all the vertices and set the color for them
        for(unsigned int i = 0; i < GetNumberOfVertices(); i++)
        {
            vec4 color = m_Vertices.at(i).GetColor();
            color.a = aAlpha;
            m_Vertices.at(i).SetColor(color);
        }
            
        //Update the vertex buffer
        UpdateVertexBuffer();
    }
    
    void Polygon::SetRenderMode(RenderMode aRenderMode)
    {
        m_RenderMode = aRenderMode;
    }

    RenderMode Polygon::GetRenderMode()
    {
        return m_RenderMode;
    }
    
    void Polygon::SetPointSize(float aPointSize)
    {
        m_PointSize = aPointSize;
    }
    
    float Polygon::GetPointSize()
    {
        return m_PointSize;
    }
    
    void Polygon::UpdateVertexBuffer()
    {
        //If the shader is null we can't update the vertex buffer
        if(m_Shader == nullptr)
        {
            return;
        }
        
        //Safety check the vertices
        if (GetNumberOfVertices() == 0)
        {
            return;
        }

        //Calculate the polygon's size
        CalculateSize();
    
        //If the VAO hasn't been generated, generate one
        if(m_VertexArrayObject == 0)
        {
            glGenVertexArrays(1, &m_VertexArrayObject);
        }
        
        //Bind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(m_VertexArrayObject);

        //Use the shader
        m_Shader->Use();

        //If the VBO hasn't been generated, generate one
        if(m_VertexBufferObject == 0)
        {
            glGenBuffers(1, &m_VertexBufferObject);
        }
        
        //Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObject);

        //Reset the alpha flag
        m_ContainsAlpha = false;

        //Create an array to hold the vertices
        const unsigned long vertexCount = GetNumberOfVertices();
        const unsigned long vertexSize = 2;
        const unsigned long colorSize = 4;
        float* vertices = new float[vertexCount * (vertexSize + colorSize)];
        
        //Cycle through and set the vertices and their colors
        for (unsigned int i = 0; i < vertexCount; i++)
        {
            //Copy the vertices into the array that we will pass to OpenGL
            int offset = i * (vertexSize + colorSize);
            size_t numBytes = (vertexSize + colorSize) * sizeof(float);
            memcpy(&vertices[offset], m_Vertices.at(i).GetArray(), numBytes);

            //Check to see if any of the vertices have an alpha and will need blending
            if (m_Vertices.at(i).GetColor().a < 1.0f)
            {
                m_ContainsAlpha = true; 
            }
        }
        
        //Set the vertex data
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * (vertexSize + colorSize), vertices, GL_STATIC_DRAW);
        
        //Enable the shader's vertices attribute
        int verticesIndex = m_Shader->GetAttribute("a_vertices");
        glEnableVertexAttribArray(verticesIndex);

        //Set the shader's vertices attribute
        long verticesOffset = 0;
        glVertexAttribPointer(verticesIndex, vertexSize, GL_FLOAT, GL_FALSE, sizeof(float) * (vertexSize + colorSize), (const GLvoid*)verticesOffset);
        
        //Enable the shader's color attribute
        int colorIndex = m_Shader->GetAttribute("a_sourceColor");
        glEnableVertexAttribArray(colorIndex);
        
        //Set the shader's colors attribute
        long colorOffset = vertexSize * sizeof(float);
        glVertexAttribPointer(colorIndex, colorSize, GL_FLOAT, GL_FALSE, sizeof(float) * (vertexSize + colorSize), (const GLvoid*)colorOffset);
        
        //Delete the vertices array
        SafeDeleteArray(vertices);
        
        //Unbind the VAO
        ServiceLocator::GetGraphics()->BindVertexArray(0);

        //The vertex buffer is no longer dirty
        m_VertexBufferDirty = false;
    }

    void Polygon::CalculateSize()
    {
        //Local variables used below
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;

        //Cycle through and set the left, right, bottom and top vars
        for (unsigned int i = 0; i < m_Vertices.size(); i++)
        {
            left = fminf(left, m_Vertices.at(i).GetPosition().x);
            right = fmaxf(right, m_Vertices.at(i).GetPosition().x);
            bottom = fminf(bottom, m_Vertices.at(i).GetPosition().y);
            top = fmaxf(top, m_Vertices.at(i).GetPosition().y);
        }

        //Set the width and height
        m_Size.x = right - left;
        m_Size.y = top - bottom;
    }
}