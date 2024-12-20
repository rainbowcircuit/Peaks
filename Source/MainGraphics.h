/*
  ==============================================================================

    MainGraphics.h
    Created: 5 Oct 2024 12:17:47pm
    Author:  Takuma Matsui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class MainGraphics : public juce::Component, juce::AudioProcessorParameter::Listener
{
public:
    MainGraphics(std::array<std::atomic<float>, 8>& _bandLevel);
    ~MainGraphics();
    //==============================================================================
    void paint(juce::Graphics& g) override;
    
    //==============================================================================
    void ratioValues();
    void ampValues();

    void updateValues(int newPartials,float newResonance, float newScale, float newOffset, float newRound, float newAmp, bool newLatch);
    //==============================================================================
    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};
        
    int partials;
    float resonance, scale, offset, round, amp;
    bool latch;

private:

    juce::Atomic<bool> parametersChanged { false };
    //==============================================================================
    std::array<std::atomic<float>, 8>& _bandLevel;
    std::array<float, 8> ratioValNormalized;
    std::array<float, 8> ampValNormalized;

    std::array<float, 8> peakValuesX;
    std::array<float, 8> peakValuesY = { 0.3f, 0.8f, 0.1f, 0.0f, 0.9f, 0.6f, 0.4f, 0.5f};
    //==============================================================================
    std::array<std::atomic<float>, 8> peakLevelMeasurement { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    //==============================================================================
    PeaksAudioProcessor audioProcessor;
};
