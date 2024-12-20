/*
  ==============================================================================

    Graphics.h
    Created: 11 Nov 2024 8:02:36pm
    Author:  Takuma Matsui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================

class GraphicsListener
{
public:
    virtual ~GraphicsListener() = default;
    virtual void hoverState(bool hover) = 0;
    virtual void dragState(bool drag) = 0;
    virtual void labelState(bool label) = 0;
};



class Graphics : public juce::Component, juce::Slider::Listener, juce::Timer// , juce::Label::Listener
{
public:
    Graphics(int width, int height, const juce::String& paramName, juce::LookAndFeel_V4& newLookAndFeel);
    ~Graphics() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    //==============================================================================
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    
//    bool hitTest(int x, int y) override;
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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Graphics)
};


//==================================================================================

class LFOGraphics : public juce::Component, juce::AudioProcessorParameter::Listener
{
public:
    float lfoRate = 3.0f, lfoAmp = 30.0f, lfoMode;
    
    LFOGraphics();
    ~LFOGraphics();
    void paint(juce::Graphics& g) override;
    void updateValues(float newMode, float newRateInHz, float newRateInBPM, float newRateInRatio, float newAmp);
    
    
private:
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};
    
    juce::Atomic<bool> parametersChanged { false };
    PeaksAudioProcessor audioProcessor;

};

