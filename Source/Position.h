
/*
  ==============================================================================

    DialGraphics.h
    Created: 25 Sep 2024 5:49:42pm
    Author:  Takuma Matsui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Graphics.h"

class PositionGraphics : public juce::LookAndFeel_V4
{
public:
    
    PositionGraphics()
    {
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(100, 99, 102));
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
        
        juce::Path graphicPath, graphicLeftPath, graphicRightPath, graphicRightBackPath, shadowPath;
        //==============================================================================
        float isoSkewFactor = 2.0f;
        float boxWidth = bounds.getWidth() * 0.95f;
        float boxHeight = bounds.getHeight() * 0.95f;
        
        
        float rotationFactor = (sliderPosProportional * 0.8 - 0.4f) * 1.57f;
        float topHeightOffset = bounds.getCentreX() - boxHeight/2;
        float botHeightOffset = bounds.getCentreX() + boxHeight/2;
        

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

        juce::PathStrokeType strokeType(0.75f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
        
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

        g.setColour(juce::Colour(100, 100, 100));
        g.fillPath(graphicRightPath);
        g.strokePath(graphicRightPath, strokeType);

        // top panel
        graphicPath.startNewSubPath(topLeftCoords);
        graphicPath.lineTo(topCentreTopCoords);
        graphicPath.lineTo(topRightCoords);
        graphicPath.lineTo(topCentreBotCoords);
        graphicPath.closeSubPath();
        graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
        g.setColour(juce::Colour(233, 193, 89));
        g.fillPath(graphicPath);
        g.strokePath(graphicPath, strokeType);

    }
    
    
    
    /*
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);

        
        
        float posInverted = 1.0f - sliderPosProportional;
        float graphicPos = (posInverted * bounds.getHeight() * 0.6f) + (bounds.getHeight() * 0.15f);
        float graphicPosMargin = bounds.getHeight() * 0.15f;
        float graphicPosFolded = 0.5f - abs(sliderPosProportional - 0.5f);
        
        float graphicMinWidth = bounds.getWidth() * 0.075f;
        float graphicMaxWidth = (bounds.getWidth() * 0.2f) + (bounds.getWidth() * 0.8f * graphicPosFolded);
        float graphicHeight = bounds.getHeight() * 1.0f;
        float graphicRadius = graphicMinWidth * 2;
        
        //==============================================================================

        std::array<float, 2> topLeftCoords = { bounds.getCentreX() - graphicMinWidth,
                                            bounds.getCentreY() - graphicHeight/2 };
        
        std::array<float, 2> topRightCoords = { bounds.getCentreX() + graphicMinWidth,
                                                bounds.getCentreY() - graphicHeight/2 };
        
        std::array<float, 2> botLeftCoords = { bounds.getCentreX() - graphicMinWidth,
                                                bounds.getCentreY() + graphicHeight/2 };
        
        std::array<float, 2> botRightCoords = { bounds.getCentreX() + graphicMinWidth,
                                                bounds.getCentreY() + graphicHeight/2 };
        
        std::array<float, 2> midLeftCoords = { bounds.getCentreX() - graphicMaxWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos };
        
        std::array<float, 2> midRightCoords = { bounds.getCentreX() + graphicMaxWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos };
        
        std::array<float, 2> midTopLeftCoords = { bounds.getCentreX() - graphicMinWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos - graphicPosMargin};
        
        std::array<float, 2> midTopRightCoords = { bounds.getCentreX() + graphicMinWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos - graphicPosMargin};
        
        std::array<float, 2> midBotLeftCoords = { bounds.getCentreX() - graphicMinWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos + graphicPosMargin };
        
        std::array<float, 2> midBotRightCoords = { bounds.getCentreX() + graphicMinWidth,
                                                (bounds.getY() + graphicMinWidth * 2) + graphicPos + graphicPosMargin };

        //==============================================================================
        
        juce::Path graphicPath, arrowPath;
        
        //==============================================================================
                
        graphicPath.startNewSubPath(topLeftCoords[0], topLeftCoords[1]);
        
        graphicPath.addArc(topLeftCoords[0], topLeftCoords[1],
                           graphicRadius, graphicRadius,
                           4.71f, 7.85f);
        
        graphicPath.lineTo(topRightCoords[0], topRightCoords[1]);
        
        graphicPath.lineTo(midTopRightCoords[0], midTopRightCoords[1]);
        graphicPath.lineTo(midRightCoords[0], midRightCoords[1]);
        graphicPath.lineTo(midBotRightCoords[0], midBotRightCoords[1]);
        
        graphicPath.lineTo(botRightCoords[0], botRightCoords[1] - graphicRadius);
        graphicPath.addArc(botLeftCoords[0], botLeftCoords[1]  - graphicRadius,
                           graphicRadius, graphicRadius,
                           1.57f, 4.71f);
        
        graphicPath.lineTo(botLeftCoords[0], botLeftCoords[1] - graphicRadius);
        
        graphicPath.lineTo(midBotLeftCoords[0], midBotLeftCoords[1]);
        graphicPath.lineTo(midLeftCoords[0], midLeftCoords[1]);
        graphicPath.lineTo(midTopLeftCoords[0], midTopLeftCoords[1]);
        
        graphicPath.closeSubPath();
        graphicPath = graphicPath.createPathWithRoundedCorners(20.0f);

        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }

        g.fillPath(graphicPath);
        
        //==============================================================================

        arrowPath.startNewSubPath(midLeftCoords[0], midLeftCoords[1]);
        arrowPath.lineTo(midLeftCoords[0] - 6.0f, midLeftCoords[1] + 3.0f);
        arrowPath.lineTo(midLeftCoords[0] - 6.0f, midLeftCoords[1] - 3.0f);
        arrowPath.closeSubPath();
        g.setColour(juce::Colour(150, 150, 150)); //
        arrowPath = arrowPath.createPathWithRoundedCorners(2.0f);

        g.fillPath(arrowPath);
    }
     */

    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {
        g.setColour(juce::Colour(100, 100, 100));
        juce::Path bgPath;
   //     bgPath.addRoundedRectangle(0, 0, width, height, 4.0f);
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

