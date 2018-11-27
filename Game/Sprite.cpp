//
//  Sprite.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2014-03-05.
//  Copyright (c) 2014 Algonquin College. All rights reserved.
//

#include "Sprite.h"
#include "../Source/Graphics/Textures/TextureFrame.h"
#include "../Source/Services/ServiceLocator.h"
#include "../Source/Platforms/PlatformLayer.h"


namespace GameDev2D
{
    Sprite::Sprite(const string& aFilename, const string& aAtlasKey) : GameObject("Sprite"),
        m_Texture(nullptr),
        m_Color(Color::WhiteColor())
    {
        //Set the Sprite's texture
        Set(aFilename, aAtlasKey);

        //Reset the model matrix
        ResetModelMatrix();
    }

    Sprite::Sprite() : GameObject("Sprite"),
        m_Texture(nullptr),
        m_Color(Color::WhiteColor())
    {

    }

    Sprite::~Sprite()
    {
        //Set the shader to null
        m_Shader = nullptr;

        //Delete the texture object
        if (m_Texture != nullptr)
        {
            delete m_Texture;
            m_Texture = nullptr;
        }
    }

    void Sprite::Draw()
    {
        if (IsEnabled() == true)
        {
            //Safety check the shader
            if (m_Shader == nullptr)
            {
                //An error occured, we can't draw the sprite, exit the function early
                return;
            }

            //Ensure there is a texture to Draw
            if (m_Texture != nullptr)
            {
                //Is the model matrix dirty?
                if (IsModelMatrixDirty() == true)
                {
                    ResetModelMatrix();
                }

                //Draw the texture
                m_Texture->Draw(m_ModelMatrix);
            }

            //Ensure the GameObject's Draw methid is called, it takes care of the Debug Drawing
            GameObject::Draw();
        }
    }

    float Sprite::GetWidth()
    {
        if (m_Texture != nullptr)
        {
            return m_Texture->GetSourceFrame().size.x;
        }
        return 0.0f;
    }

    float Sprite::GetHeight()
    {
        if (m_Texture != nullptr)
        {
            return m_Texture->GetSourceFrame().size.y;
        }
        return 0.0f;
    }

    void Sprite::SetAlpha(float aAlpha)
    {
        if (m_Color.Alpha() != aAlpha)
        {
            //Set the alpha
            m_Color.SetAlpha(aAlpha);

            //Safety check the texture
            if (m_Texture != nullptr)
            {
                m_Texture->SetColor(m_Color);
            }
        }
    }

    float Sprite::GetAlpha()
    {
        return m_Color.Alpha();
    }

    void Sprite::Set(const string& aFilename, const string& aAtlasKey)
    {
        //Check first to see if there is already a texture object, we would 
        //need to delete it, before creating a new one
        if (m_Texture != nullptr)
        {
            delete m_Texture;
            m_Texture = nullptr;
        }

        //Create the texture frame object
        m_Texture = new TextureFrame(aFilename, aAtlasKey);
        m_Texture->SetShader(m_Shader);
        m_Texture->SetColor(m_Color);
    }
}