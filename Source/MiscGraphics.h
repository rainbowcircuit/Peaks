#pragma once
#include "LookAndFeel.h"

class RoundGraphics : public juce::LookAndFeel_V4
{
public:
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        //==============================================================================
        juce::Path sliderBGPath, sliderValuePath;
        juce::PathStrokeType strokeType(6.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

        sliderBGPath.startNewSubPath(bounds.getX(), bounds.getCentreY());
        sliderBGPath.lineTo(bounds.getRight(), bounds.getCentreY());
        g.setColour(Colours::darkGrey);
        g.strokePath(sliderBGPath, strokeType);
        
        sliderValuePath.startNewSubPath(bounds.getX(), bounds.getCentreY());
        sliderValuePath.lineTo(bounds.getX() + (bounds.getWidth() * sliderPosProportional), bounds.getCentreY());
        g.setColour(Colours::baseYellow);
        g.strokePath(sliderValuePath, strokeType);

    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {
        g.setColour(Colours::darkGrey);
        juce::Path bgPath;
        bgPath.addRoundedRectangle(textEditor.getLocalBounds().reduced(4.0f, 0.0f), 3.0f);
        g.fillPath(bgPath);
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto label = LookAndFeel_V4::createSliderTextBox(slider);
        
        label->setFont(Fonts::getFont(13.0f));
        label->setJustificationType(juce::Justification::centred);
        

        label->onEditorShow = [label]()
        {
            if(auto* editor = label->getCurrentTextEditor())
            {
                editor->setJustification(juce::Justification::centred);
            }
            
        };
        return label;
    }
     
};

class StereoGraphics : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        juce::Path graphicLPath, graphicRPath, sliderPath, sliderBGPath, bgPath;
        
        // background
        bgPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 3.0f);
        g.setColour(Colours::lightGrey);
        g.fillPath(bgPath);
        
        // slider
        float sliderBotPos = bounds.getY() + (bounds.getHeight() * 0.9f);
        float sliderHeight = bounds.getHeight() * 0.1f;
        float sliderValue = bounds.getWidth() * sliderPosProportional;
        sliderPath.addRoundedRectangle(bounds.getX(), sliderBotPos, sliderValue, sliderHeight, 3.0f, 3.0f, false, false, true, true);
        sliderBGPath.addRoundedRectangle(bounds.getX(), sliderBotPos, bounds.getWidth(), sliderHeight, 3.0f, 3.0f, false, false, true, true);
        g.setColour(Colours::lightGreyShadow);
        g.fillPath(sliderBGPath);
        g.setColour(Colours::baseYellow);
        g.fillPath(sliderPath);
        
        // drawStereo
        float graphicRadius = (bounds.getWidth() * ((1.0f - sliderPosProportional) * 0.15f + 0.35))/2;
        float graphicPos = bounds.getWidth() * (sliderPosProportional * 0.125);
        graphicLPath.addCentredArc(bounds.getCentreX() - graphicPos, bounds.getCentreY(), graphicRadius, graphicRadius, 0.0f, 0.0f, 6.28f, true);
        graphicRPath.addCentredArc(bounds.getCentreX() + graphicPos, bounds.getCentreY(), graphicRadius, graphicRadius, 0.0f, 0.0f, 6.28f, true);
        g.setColour(Colours::darkGrey);
        g.strokePath(graphicLPath, juce::PathStrokeType(1.0f));
        g.strokePath(graphicRPath, juce::PathStrokeType(1.0f));
    }
};

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
        auto bgColor = state ? Colours::baseYellow : Colours::lightGrey;
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

            g.setColour(Colours::darkGrey);
            g.strokePath(graphicsPath, juce::PathStrokeType(1.0f));
        }
    }
};

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
        auto bgColor = state ? Colours::baseYellow : Colours::lightGrey;
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
        g.setColour(Colours::darkGrey);
        g.strokePath(framePath, juce::PathStrokeType(1.0f));
        g.strokePath(keyPath, juce::PathStrokeType(1.0f));
        g.strokePath(divPath, juce::PathStrokeType(1.0f));
    }
};

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
        auto bgColor = state ? Colours::baseYellow : Colours::lightGrey;
        g.setColour(bgColor);
        g.fillPath(bgPath);
        g.setColour(juce::Colour(0, 0, 0));
        g.setFont(10.0f);
        
        // drawText
        g.drawText(labelText, bounds, juce::Justification::centred, false);
        g.setColour(Colours::darkGrey);
        g.strokePath(graphicPath, juce::PathStrokeType(1.5f));
    }
    juce::String labelText;
};

class TextDisplaySliderGraphics : public juce::LookAndFeel_V4
{
public:
    TextDisplaySliderGraphics(juce::String suffix, float newFontSize, juce::Colour newColour, juce::Justification newJustification) :
    paramSuffix(suffix), fontSize(newFontSize), textColour(newColour), justification(newJustification) {}

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        float valueNum = slider.getValue();
        juce::String valueText = slider.getTextFromValue(valueNum);
        juce::String labelText = valueText + paramSuffix;

        juce::Path bgPath;
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(overlayColour);
        
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(textColour);
        }

        g.setFont(fontSize);
        g.drawText(labelText, bounds, justification, false);
    }
    
private:
    juce::String paramSuffix;
    juce::String labelText;
    float fontSize;
    juce::Colour
        textColour,
        overlayColour = textColour.withAlpha(0.75f);
    
    juce::Justification justification;
};
