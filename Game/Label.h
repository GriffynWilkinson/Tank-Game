//
//  Label.h
//  GameDev2D
//
//  Created by Bradley Flood on 2013-08-13.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef __GameDev2D__Label__
#define __GameDev2D__Label__

#include "GameObject.h"
#include "../Source/Graphics/Fonts/TrueTypeFont.h"
#include "../Source/Graphics/Core/Color.h"


using namespace glm;
using namespace std;

namespace GameDev2D
{
    //Forward declarations
    class Shader;
    class RenderTarget;
    class TextureFrame;

    //Label text can be left, center and right justified
    enum LabelJustification
    {
        JustifyLeft = 0,
        JustifyCenter,
        JustifyRight
    };

    //Creates a label from a .ttf or .otf font file. The label can be transform (translated, rotatated, scaled)
    class Label : public GameObject
    {
    public:
        //Contructs a Label using a TrueTypeFont, the font size must be specified
        Label(const string& trueTypeFont, const string& extension, const unsigned int fontSize);
        ~Label();

        //Draws the Label
        void Draw();

        //Sets the text that the Label will display
        void SetText(const string& text);

        //Returns the text the Label is displaying
        string GetText();

        //Returns the size of the label
        float GetWidth();
        float GetHeight();

        //Sets the color of the Label
        void SetColor(Color color);

        //Returns the Color of the Label
        Color GetColor();

        //Sets the alpha for the Label
        void SetAlpha(float alpha);

        //Returns the alpha of the Label
        float GetAlpha();

        //Set the angle of the characters
        void SetCharacterAngle(float characterAngle);

        //Returns the angles of the characters
        float GetCharacterAngle();

        //Set the spacing between each character
        void SetCharacterSpacing(float characterSpacing);

        //Returns the spacing between each character
        float GetCharacterSpacing();

        //Sets the justification of the Label
        void SetJustification(LabelJustification justification);

        //Returns the justification of the Label
        LabelJustification GetJustification();

        //Returns how many lines make up the Label
        unsigned long GetNumberOfLines();

    protected:
        //Conveniance methods to resize and reset the render target
        void ResizeRenderTarget();
        void ResetRenderTarget();

        //Conveniance method to calculate the size of the Label, based on the text
        vec2 CalculateSize();

        //Conveniance method to draw the text
        void DrawText();

        //Conveniance method to draw an individual character
        void DrawCharacter(TextureFrame* textureFrame, vec2 position);

        //Member variables
        Font* m_Font;
        RenderTarget* m_RenderTarget;
        LabelJustification m_Justification;
        string m_Text;
        vec2 m_Size;
        Color m_Color;
        float m_CharacterAngle;
        float m_CharacterSpacing;
        vector<float> m_LineWidth;
    };
}

#endif /* defined(__GameDev2D__Label__) */
