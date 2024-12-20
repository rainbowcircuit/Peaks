

#pragma once
#include <JuceHeader.h>

class LatchGraphics : public juce::LookAndFeel_V4
{
public:

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto boundsNoMargin = toggleButton.getLocalBounds().toFloat();
        auto bounds = boundsNoMargin.reduced(5, 5);
        auto graphicsBounds = bounds.reduced(3, 3);
        bool state = toggleButton.getToggleState();
        
        juce::Path graphicsPath, bgPath;

        bgPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 3.0f);
        auto bgColor = state ? juce::Colour(243, 203, 99) : juce::Colour(200, 200, 200);
        g.setColour(bgColor);
        g.fillPath(bgPath);
        
        for(int i = 0; i < 3; i++){
            
            juce::Point<float> leftCoord = { graphicsBounds.getX() + (graphicsBounds.getWidth()/6) * i, graphicsBounds.getY() + graphicsBounds.getHeight() };
            juce::Point<float> topCoord = { graphicsBounds.getX() + (graphicsBounds.getWidth()/6) * (i + 2), graphicsBounds.getY() };
            juce::Point<float> rightCoord = { graphicsBounds.getX() + (graphicsBounds.getWidth()/6) * (i + 4), graphicsBounds.getY() + graphicsBounds.getHeight() };

            graphicsPath.startNewSubPath(leftCoord);
            graphicsPath.lineTo(topCoord);
            graphicsPath.lineTo(rightCoord);
            graphicsPath = graphicsPath.createPathWithRoundedCorners(2.0f);

            g.setColour(juce::Colour(100, 99, 102));
            g.strokePath(graphicsPath, juce::PathStrokeType(1.0f));
        }
        
    }
};

//======================================================================================

class InputGraphics : public juce::LookAndFeel_V4
{
public:
    
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto boundsNoMargin = toggleButton.getLocalBounds().toFloat();
        auto bounds = boundsNoMargin.reduced(5, 5);
        auto keyboard = bounds.reduced(3, 3);

        bool state = toggleButton.getToggleState();
        float keyDiv = keyboard.getWidth()/10;

        juce::Path bgPath, framePath, keyPath, divPath;
        bgPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 3.0f);
        auto bgColor = state ? juce::Colour(243, 203, 99) : juce::Colour(200, 200, 200);
        g.setColour(bgColor);
        g.fillPath(bgPath);

        // back keys
        framePath.addRectangle(keyboard.getX(), keyboard.getY(), keyboard.getWidth(), keyboard.getHeight()); // square
        
        keyPath.addRectangle(keyboard.getX() + keyDiv * 2, keyboard.getY(), keyDiv * 2, keyboard.getHeight() * 0.7f);
        keyPath.addRectangle(keyboard.getX() + keyDiv * 6, keyboard.getY(), keyDiv * 2, keyboard.getHeight() * 0.7f);

        divPath.startNewSubPath(keyboard.getX() + keyDiv * 3, keyboard.getY() + keyboard.getHeight() * 0.7f);
        divPath.lineTo(keyboard.getX() + keyDiv * 3, keyboard.getY() + keyboard.getHeight());

        divPath.startNewSubPath(keyboard.getX() + keyDiv * 7, keyboard.getY() + keyboard.getHeight() * 0.7f);
        divPath.lineTo(keyboard.getX() + keyDiv * 7, keyboard.getY() + keyboard.getHeight());

        // front keys
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(framePath, juce::PathStrokeType(1.0f));
        g.strokePath(keyPath, juce::PathStrokeType(1.0f));
        g.strokePath(divPath, juce::PathStrokeType(1.0f));
    }
};

//======================================================================================

class TextButtonGraphics : public juce::LookAndFeel_V4
{
public:
    TextButtonGraphics(juce::String text) : labelText(text)
    {
        labelText = text;
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggleButton, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto boundsNoMargin = toggleButton.getLocalBounds().toFloat();
        auto bounds = boundsNoMargin.reduced(5, 5);
        bool state = toggleButton.getToggleState();
        
        juce::Path graphicPath, bgPath;
        
        // background
        bgPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 3.0f);
        auto bgColor = state ? juce::Colour(233, 193, 89) : juce::Colour(200, 200, 200);
        g.setColour(bgColor);
        g.fillPath(bgPath);
        g.setColour(juce::Colour(0, 0, 0));
        g.setFont(10.0f);
        
        // drawText
        g.drawText(labelText, bounds, juce::Justification::centred, false);
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(graphicPath, juce::PathStrokeType(1.5f));
    }
    juce::String labelText;
};
