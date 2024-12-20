

#pragma once
#include <JuceHeader.h>

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
