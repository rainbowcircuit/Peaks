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

class ResonanceGraphics : public juce::LookAndFeel_V4
{
public:
    
    ResonanceGraphics()
    {
        setColour(juce::Slider::textBoxTextColourId, juce::Colour(238, 234, 222));
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
            auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
            auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
            
        juce::Path graphicPath, bgPath;
        //==============================================================================
        
        float isoSkewFactor = 2.5f;
        
        float graphicHeight = bounds.getHeight() * 0.85f;
        juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

       for (int i = 0; i < 6; i++){
           float bgIncrement = i * (bounds.getWidth()/12);
           bgPath.startNewSubPath((bounds.getCentreX() - bounds.getWidth()/2) + bgIncrement, (bounds.getCentreY() - graphicHeight/4) - (bgIncrement/isoSkewFactor));
           bgPath.lineTo(bounds.getCentreX() + bgIncrement, (bounds.getCentreY() + graphicHeight/2) - (bgIncrement/isoSkewFactor));

           g.setColour(juce::Colour(238, 234, 222).withAlpha(0.15f));
           g.strokePath(bgPath, strokeType);
           
           //==============================================================================
           
           float fgIncrement = (sliderPosProportional * 0.75f + 0.25f) * i * (bounds.getWidth()/12);
           graphicPath.startNewSubPath((bounds.getCentreX() - bounds.getWidth()/2) + fgIncrement, (bounds.getCentreY() - graphicHeight/4) - (fgIncrement/isoSkewFactor));
           graphicPath.lineTo(bounds.getCentreX() + fgIncrement, (bounds.getCentreY() + graphicHeight/2) - (fgIncrement/isoSkewFactor));
           
           if (slider.isMouseOverOrDragging()){
               slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
               g.setColour(juce::Colour(243, 203, 99));
           } else {
               slider.setMouseCursor(juce::MouseCursor::NormalCursor);
               g.setColour(juce::Colour(233, 193, 89));
           }
           g.strokePath(graphicPath, strokeType);
        }
    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {}
    
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override
    {
        juce::Slider::SliderLayout layout;        
        layout.textBoxBounds = juce::Rectangle<int> (0, 25, 60, 15);
        layout.sliderBounds = juce::Rectangle<int> (60, 0, 60, 50);
        
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

private:
};

