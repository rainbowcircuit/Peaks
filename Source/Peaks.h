#pragma once
#include <JuceHeader.h>
#include "StateVariableFilter.h"
#include "DelayLine.h"
#include "LFO.h"
#include "Utility.h"

class Peaks
{
public:
    // member variables
    float cutoff, resonance, position, scale = 1.0f, offset, round, amplitude, mix, stereo;
    int partials = 8;
    bool latch;
    std::array<std::atomic<float>, 8> bandLevelMeasurements = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    //==============================================================================
    void prepareToPlay(float sampleRate, juce::AudioPlayHead* playhead);
    
    //==============================================================================
    void setPeaksValues(int newPartials, float newCutoff, float newResonance, float newMix, float newStereo, bool newLatch);
    void setRatioValues(float newScale, float newOffset, float newRound);
    void setStiffnessValues(float newPosition, float newStructure);
    void setAmpValue(float newAmplitude);
    void setFMValues(int newMode, float newRateInHz, float newRateInBPM, float newRateInRatio, float newAmp);

    //==============================================================================
    float positionProcessSample(float input, float amount, int channel) noexcept;
    float softClipProcessSample(float input) noexcept;
    
    //==============================================================================    
    void processBlock(juce::AudioBuffer<float>& buffer) noexcept;
    //==============================================================================
    
private:
    float peaksOutput = 0.0f;
    float sampleRate;
    
    std::array<float, 8> ratioVal;
    std::array<float, 8> ampVal;
    
    const float maxFrequencyInHz = 8000.0f;
    const float minFrequencyInHz = 50.0f;

    float lfoMode, lfoRateInHz, lfoRateInBPM, lfoRateInRatio, lfoAmp;
    
 //   juce::dsp::DelayLine<float> combDelayLineL, combDelayLineR;
    std::array<SVF, 2> svfLP;

    DCBlock outputDCBlockL, outputDCBlockR;
    std::array<DelayLine, 2> combDelay;
    std::array<SVF, 8> svfBP_L, svfBP_R;
    std::array<DelayLine, 8> delays;
    LFO lfo;
    
  //  std::array<LFO, 8> lfos;

};

