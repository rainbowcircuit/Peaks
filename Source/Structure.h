/*
  ==============================================================================

    DialGraphics.h
    Created: 25 Sep 2024 5:49:42pm
    Author:  Takuma Matsui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utility.h"
#include "Graphics.h"

class StructureGraphics : public juce::LookAndFeel_V4
{
public:
    
    StructureGraphics()
    {
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(238, 234, 222));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

    }

    auto returnClassName()
    {
        return "StructureGraphics";
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
            
        juce::Path graphicPath, graphicLeftPath, graphicRightPath, graphicRightBackPath, highlightPath;
        
        //==============================================================================
        float isoSkewFactor = 2.0f;
        float boxWidth = bounds.getHeight() * 1.5f;
        float boxHeight = bounds.getHeight() * 0.5f;
            
        float rotationFactor = sliderPosProportional * 1.57f;
        float topHeightOffset = bounds.getCentreY() - boxHeight/2;
        float botHeightOffset = bounds.getCentreY() + boxHeight;
            

        juce::Point topLeftCoords = { bounds.getCentreX() - (cos(rotationFactor) * boxWidth/2),
        topHeightOffset -  (sin(rotationFactor) * boxHeight/2/isoSkewFactor) };
        juce::Point topCentreBotCoords = { bounds.getCentreX() - (cos(rotationFactor - 1.57f) * boxWidth/2),
        topHeightOffset -  (sin(rotationFactor - 1.57f) * boxHeight/2/isoSkewFactor) };
        juce::Point topRightCoords = { bounds.getCentreX() + (cos(rotationFactor) * boxWidth/2),
        topHeightOffset + (sin(rotationFactor) * boxHeight/2/isoSkewFactor) };
        juce::Point topCentreTopCoords = { bounds.getCentreX() + (cos(rotationFactor - 1.57f) * boxWidth/2),
        topHeightOffset +  (sin(rotationFactor - 1.57f) * boxHeight/2/isoSkewFactor) };

        juce::Point botLeftCoords = { bounds.getCentreX() - boxWidth/2, botHeightOffset - (boxHeight/2) };
        juce::Point botCentreBotCoords = { bounds.getCentreX(), botHeightOffset - (boxHeight/2/isoSkewFactor) };
        juce::Point botRightCoords = { bounds.getCentreX() + boxWidth/2, botHeightOffset - (boxHeight/2) };
        juce::Point botCentreTopCoords = { bounds.getCentreX(), botHeightOffset - (boxHeight/2/isoSkewFactor) };
            
        //==============================================================================

        juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
            
        //==============================================================================
        // left panel
        graphicLeftPath.startNewSubPath(botLeftCoords);
        graphicLeftPath.lineTo(botCentreBotCoords);
        graphicLeftPath.lineTo(topCentreBotCoords);
        graphicLeftPath.lineTo(topLeftCoords);
        graphicLeftPath.closeSubPath();
        graphicLeftPath = graphicLeftPath.createPathWithRoundedCorners(2.0f);

        g.setColour(juce::Colour(238, 234, 222));
        g.fillPath(graphicLeftPath);
        g.strokePath(graphicLeftPath, strokeType);
            
        // right Panel
        graphicRightPath.startNewSubPath(botRightCoords);
        graphicRightPath.lineTo(botCentreBotCoords);
        graphicRightPath.lineTo(topCentreBotCoords);
        graphicRightPath.lineTo(topRightCoords);
        graphicRightPath.closeSubPath();
        graphicRightPath = graphicRightPath.createPathWithRoundedCorners(2.0f);

        g.setColour(juce::Colour(100, 99, 102));
        g.fillPath(graphicRightPath);
        g.strokePath(graphicRightPath, strokeType);

        
        // highlight panel
        highlightPath.startNewSubPath(topRightCoords);
        highlightPath.lineTo(topCentreBotCoords);
        highlightPath.lineTo(botCentreBotCoords);
        highlightPath.closeSubPath();
        graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
        g.setColour(juce::Colour(255, 255, 255).withAlpha(sliderPosProportional * 0.15f));
        g.fillPath(highlightPath);
        g.strokePath(highlightPath, strokeType);

        // top panel
        graphicPath.startNewSubPath(topLeftCoords);
        graphicPath.lineTo(topCentreTopCoords);
        graphicPath.lineTo(topRightCoords);
        graphicPath.lineTo(topCentreBotCoords);
        graphicPath.closeSubPath();
        graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
        
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }
        
        g.fillPath(graphicPath);
        g.strokePath(graphicPath, strokeType);
        
    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {}
    
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override
    {
        juce::Slider::SliderLayout layout;
        layout.textBoxBounds = juce::Rectangle<int> (0, 25, 60, 15);
        layout.sliderBounds = juce::Rectangle<int> (60, 0, 70, 50);
        
        return layout;
    }
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        auto label = LookAndFeel_V4::createSliderTextBox(slider);
        
        label->setFont(juce::Font(14.0f, juce::Font::plain));
        label->setJustificationType(juce::Justification::right);
        

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
