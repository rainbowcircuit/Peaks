#pragma once
#include <JuceHeader.h>
#include "LookAndFeel.h"

class GUIGraphics : public juce::Component, juce::Slider::Listener, juce::Timer// , juce::Label::Listener
{
public:
    GUIGraphics(int width, int height, const juce::String& paramName, juce::LookAndFeel_V4& newLookAndFeel);
    ~GUIGraphics() override;
    
    //==============================================================================
    void resized() override;
    
    //==============================================================================
    juce::Slider slider;
private:
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
    
    //==============================================================================
    double sliderValue;
    juce::Label nameLabel, valueLabel;
    int width, height;
    juce::LookAndFeel_V4& lookAndFeel;
    bool hover = false;
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GUIGraphics)
};

class StructureGraphics : public juce::LookAndFeel_V4
{
public:
    StructureGraphics();
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override {}
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override {}
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
    
private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StructureGraphics)
};

class ResonanceGraphics : public juce::LookAndFeel_V4
{
public:
    ResonanceGraphics();
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override {}
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override {}
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResonanceGraphics)

};

class GreyDialGraphics : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override {}
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
};

class OrangeDialGraphics : public juce::LookAndFeel_V4
{
public:
    OrangeDialGraphics(bool external) : externalMIDI(external) {}
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;
    
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& texteditor) override {}
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
    juce::Label* createSliderTextBox(juce::Slider& slider) override;

private:
    bool externalMIDI;
};

