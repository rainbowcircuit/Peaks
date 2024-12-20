#pragma once
#include <JuceHeader.h>

class SVF
{
public:
    void setSampleRate(float sampleRate);
    void setCoefficients(float freq, float Q);
    float processSample(float input, int filterType) noexcept;
    void reset();

private:
    float sampleRate = 48000.0f;
    float cutoff;
    float resonance;
    
    // coefficients
    float g, k, a1, a2, a3;
    // unit delays
    float z1, z2;
    juce::dsp::FastMathApproximations fastTan;
};

