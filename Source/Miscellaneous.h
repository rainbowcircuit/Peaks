#pragma once
#include "Resonance.h"
#include "Graphics.h"

class PartialsGraphics : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        //==============================================================================
        
        juce::Path graphicsPath, bgPath;
        
        //==============================================================================
        // background
        for (int i = 1; i <= 8; i++)
        {
            float arcPosition = (bounds.getWidth()/8) * i;
            bgPath.addCentredArc(arcPosition, bounds.getCentreY(), 3.0f, 3.0f, 0.0f, 0.0f, 6.28f, true);
        }
        g.setColour(juce::Colour(100, 99, 102));
        g.fillPath(bgPath);

        //==============================================================================
        // foreground
        int sliderCount = slider.getValue();
        
        for (int i = 1; i <= sliderCount; i++)
        {
            float arcPosition = (bounds.getWidth()/8) * i;
            graphicsPath.addCentredArc(arcPosition, bounds.getCentreY(), 3.15f, 3.15f, 0.0f, 0.0f, 6.28f, true);
        }
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
            
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }
        g.fillPath(graphicsPath);
    }
};

//======================================================================================

class DryWetGraphics : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        //==============================================================================
        
        juce::Path graphicsPath, bgPath;
        
        //==============================================================================
        // background
        for (int i = 1; i <= 8; i++)
        {
            float arcPosition = (bounds.getWidth()/8) * i;
            bgPath.addCentredArc(arcPosition, bounds.getCentreY(), 3.0f, 3.0f, 0.0f, 0.0f, 6.28f, true);
        }
        g.setColour(juce::Colour(100, 99, 102));
        g.fillPath(bgPath);

        //==============================================================================
        // foreground
        int sliderCount = slider.getValue();
        
        for (int i = 1; i <= sliderCount; i++)
        {
            float arcPosition = (bounds.getWidth()/8) * i;
            graphicsPath.addCentredArc(arcPosition, bounds.getCentreY(), 3.0f, 3.0f, 0.0f, 0.0f, 6.28f, true);
        }
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
            
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }
        g.fillPath(graphicsPath);
    }
};

//======================================================================================

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
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(sliderBGPath, strokeType);
        
        sliderValuePath.startNewSubPath(bounds.getX(), bounds.getCentreY());
        sliderValuePath.lineTo(bounds.getX() + (bounds.getWidth() * sliderPosProportional), bounds.getCentreY());
        g.setColour(juce::Colour(243, 203, 99));
        g.strokePath(sliderValuePath, strokeType);

    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {
        g.setColour(juce::Colour(100, 99, 102));
        juce::Path bgPath;
        bgPath.addRoundedRectangle(textEditor.getLocalBounds().reduced(4.0f, 0.0f), 3.0f);
        g.fillPath(bgPath);
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto label = LookAndFeel_V4::createSliderTextBox(slider);
        
        label->setFont(juce::Font(12.0f, juce::Font::plain));
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

//======================================================================================

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
        g.setColour(juce::Colour(200, 200, 200));
        g.fillPath(bgPath);
        
        // slider
        float sliderBotPos = bounds.getY() + (bounds.getHeight() * 0.9f);
        float sliderHeight = bounds.getHeight() * 0.1f;
        float sliderValue = bounds.getWidth() * sliderPosProportional;
        sliderPath.addRoundedRectangle(bounds.getX(), sliderBotPos, sliderValue, sliderHeight, 3.0f, 3.0f, false, false, true, true);
        sliderBGPath.addRoundedRectangle(bounds.getX(), sliderBotPos, bounds.getWidth(), sliderHeight, 3.0f, 3.0f, false, false, true, true);
        g.setColour(juce::Colour(180, 180, 180));
        g.fillPath(sliderBGPath);
        g.setColour(juce::Colour(233, 193, 89));
        g.fillPath(sliderPath);
        
        // drawStereo
        float graphicRadius = (bounds.getWidth() * ((1.0f - sliderPosProportional) * 0.15f + 0.35))/2;
        float graphicPos = bounds.getWidth() * (sliderPosProportional * 0.125);
        graphicLPath.addCentredArc(bounds.getCentreX() - graphicPos, bounds.getCentreY(), graphicRadius, graphicRadius, 0.0f, 0.0f, 6.28f, true);
        graphicRPath.addCentredArc(bounds.getCentreX() + graphicPos, bounds.getCentreY(), graphicRadius, graphicRadius, 0.0f, 0.0f, 6.28f, true);
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(graphicLPath, juce::PathStrokeType(1.0f));
        g.strokePath(graphicRPath, juce::PathStrokeType(1.0f));



    }
};


class EnvelopeGraphics : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);

        juce::Path graphicPath, sliderPath, sliderBGPath, bgPath;
        
        // background
        bgPath.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 3.0f);
        g.setColour(juce::Colour(200, 200, 200));
        g.fillPath(bgPath);
        
        // slider
        float sliderBotPos = bounds.getY() + (bounds.getHeight() * 0.9f);
        float sliderHeight = bounds.getHeight() * 0.1f;
        float sliderValue = bounds.getWidth() * sliderPosProportional;
        sliderPath.addRoundedRectangle(bounds.getX(), sliderBotPos, sliderValue, sliderHeight, 3.0f, 3.0f, false, false, true, true);
        sliderBGPath.addRoundedRectangle(bounds.getX(), sliderBotPos, bounds.getWidth(), sliderHeight, 3.0f, 3.0f, false, false, true, true);
        g.setColour(juce::Colour(180, 180, 180));
        g.fillPath(sliderBGPath);
        g.setColour(juce::Colour(233, 193, 89));
        g.fillPath(sliderPath);
        
        // draw envelope
        float graphicSlew = bounds.getWidth() * ((1.0f - (sliderPosProportional * 0.9f)) * 0.3f);
        float graphicHeight = bounds.getHeight() * 0.25;
        float bottomWidth = bounds.getWidth() * 0.3f;
        
        graphicPath.startNewSubPath(bounds.getCentreX() - graphicSlew + 0.05f, bounds.getCentreY() - graphicHeight);
        graphicPath.lineTo(bounds.getCentreX() + graphicSlew - 0.05f, bounds.getCentreY() - graphicHeight);
        graphicPath.lineTo(bounds.getCentreX() + bottomWidth, bounds.getCentreY() + graphicHeight);
        graphicPath.lineTo(bounds.getCentreX() - bottomWidth, bounds.getCentreY() + graphicHeight);
        graphicPath.closeSubPath();
        graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
        
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(graphicPath, juce::PathStrokeType(1.0f));



    }
};
