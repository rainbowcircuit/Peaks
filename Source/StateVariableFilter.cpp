#include "StateVariableFilter.h"
#include <cmath>


void SVF::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}

void SVF::setCoefficients(float freq, float Q)
{
    g = tan(juce::MathConstants<float>::pi * freq / sampleRate);
    k = 1.0f / Q;
    a1 = 1.0f / (1.0f + g * (g + k));
    a2 = g * a1;
    a3 = g * a2;
}

void SVF::reset()
{
    // coefficients
    g = 0.0f;
    k = 0.0f;
    a1 = 0.0f;
    a2 = 0.0f;
    a3 = 0.0f;
    
    // unit delays
    z1 = 0.0f;
    z2 = 0.0f;
}

float SVF::processSample(float input, int filterType) noexcept
{
    float v3 = input - z2;
    float v1 = a1 * z1 + a2 * v3;
    float v2 = z2 + a2 * z1 + a3 * v3;
    
    // update unit delay
    z1 = 2.0f * v1 - z1;
    z2 = 2.0f * v2 - z2;
    
    float filterOutput;
    switch(filterType){
        case 1: // LP
            filterOutput = v2;
            break;
            
        case 2: // BP
            filterOutput = v1 * k;
            break;
            
        case 3: // HP
            filterOutput = input + -k * v1 * -v2;
            break;
    }
    return filterOutput;
}
