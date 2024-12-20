
#pragma once
#include <JuceHeader.h>
#include "Graphics.h"


class GreyDialGraphics : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(2.0f, 2.0f);
        
        //==============================================================================

        float pi = juce::MathConstants<float>::pi;
        float dialStart = 1.25f * pi;
        float dialEnd = 2.75f * pi;
        float sliderPositionScaled = 2.0f + (1.0f - sliderPosProportional);
        float dialPositionInRadians = dialStart + sliderPositionScaled * (dialEnd - dialStart);
        
        //==============================================================================

        juce::Path dialBodyPath, dialDotPath, dialOutlinePath;
        
        //==============================================================================

        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(110, 109, 112)); //
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(100, 99, 102)); //
        }

        // dial outline
        float dialOutlineRadius = (bounds.getWidth())/2;
        float dialBodyRadius = (bounds.getWidth() * 0.85f)/2;
        float dialDotRadius = (bounds.getWidth() * 0.6f)/2;

        dialOutlinePath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                      dialOutlineRadius, dialOutlineRadius,
                                      0.0f, dialStart, dialEnd, true);
        
        g.strokePath(dialOutlinePath, juce::PathStrokeType(1.0f));

        //==============================================================================
        // dial body
        dialBodyPath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                   dialBodyRadius, dialBodyRadius,
                                   0.0f, 0.0f, 6.28f, true);
        
        g.fillPath(dialBodyPath);
        
        //==============================================================================
        // dial dot
        std::array<float, 2> needleCoords = {bounds.getCentreX() + std::sin(dialPositionInRadians) * dialDotRadius,
                                            bounds.getCentreX() + std::cos(dialPositionInRadians) * dialDotRadius};
    
        dialDotPath.addCentredArc(needleCoords[0], needleCoords[1],
                                  1.5, 1.5, 0.0f, 0.0f, pi * 2, true);
        
        g.setColour(juce::Colour(255, 255, 255)); //
        g.fillPath(dialDotPath);
        
    }
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override
    {}
    
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override
    {
        g.setColour(juce::Colour(100, 99, 102));
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

class OrangeDialGraphics : public juce::LookAndFeel_V4
{
public:
    
    OrangeDialGraphics(bool external) : externalMIDI(external) {}
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
        
        float pi = juce::MathConstants<float>::pi;
        float dialStart = 1.25f * pi;
        float dialEnd = 2.75f * pi;
        float dialPositionInRadians = dialStart + sliderPosProportional * (dialEnd - dialStart);

        float dialRadius = bounds.getWidth() * 0.425;

        
        juce::Path sliderValuePath, sliderBGPath;
        juce::PathStrokeType strokeType(8.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

        //==============================================================================
        // bg path
        sliderBGPath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                   dialRadius, dialRadius,
                                   0.0f, dialStart, dialEnd, true);
        g.setColour(juce::Colour(100, 99, 102));
        g.strokePath(sliderBGPath, strokeType);

        //==============================================================================
        // value path
        sliderValuePath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                      dialRadius, dialRadius,
                                     0.0f, dialStart, dialPositionInRadians, true);
        
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }
        
        g.strokePath(sliderValuePath, strokeType);

        //==============================================================================
        // text
        
        
        if (!externalMIDI){
            int midiNote = log(slider.getValue()/440.0)/log(2) * 12 + 69;
            
            juce::String display = juce::MidiMessage::getMidiNoteName(midiNote, false, true, 4);
            int noteCents = (slider.getValue() - (int)slider.getValue()) * 100;
            
            g.setColour(juce::Colour(100, 99, 102));
            g.setFont(bounds.getWidth() * 0.25f);
        //    g.drawText(display, bounds, juce::Justification::centred, false);
            g.drawText(display, bounds.getX(), bounds.getY() - 4, bounds.getWidth(), bounds.getHeight(), juce::Justification::centred);
            
            g.setFont(bounds.getWidth() * 0.15f);
            g.drawText("+" + juce::String(noteCents) + " ct", bounds.getCentreX() - 20,
                       bounds.getCentreY() + 2, 40,
                       20, juce::Justification::centred);
        } else {
            juce::String display = juce::String(slider.getValue()) + " st";
            
            g.setColour(juce::Colour(100, 99, 102));
            g.setFont(bounds.getWidth() * 0.225f);
            g.drawText(display, bounds, juce::Justification::centred, false);
        }
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
    
private:
    bool externalMIDI;
};
