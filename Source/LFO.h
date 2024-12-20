#pragma once
#include <JuceHeader.h>
#include "Utility.h"

class LFO
{
public:
    void setSampleRate(float sampleRate)
    {
        this->sampleRate = sampleRate;
    }
    
    void reset()
    {
        phase = 0.0f;
        accumHz = 0.0f;
        accumSubdiv = 0.0f;
        accumRatio = 0.0f;
    }
    
    void setTimeStep(int mode, float rateInHz, int rateInBPM, float carrier, float rateInRatio)
    {
        if (mode == 0){
            accumHz += rateInHz/sampleRate;
            if (accumHz > 1.0f){
                accumHz = 0.0f;
            }
            phase = remainder(accumHz, 1.0f);
    
        } else if (mode == 1) {
            float bpmToHz = (bpm/60.0f) * subdivisionMultiplier[15 - rateInBPM];
            accumSubdiv += bpmToHz/sampleRate;
            if (accumSubdiv > 1.0f){
                accumSubdiv = 0.0f;
            }
            phase = remainder(accumSubdiv, 1.0f);
        } else if (mode == 2) {
            float modulatorFreq = carrier * rateInRatio;
            accumRatio += modulatorFreq/sampleRate;

            if (accumRatio > 1.0f){
                accumRatio = 0.0f;
            }
            phase = remainder(accumRatio, 1.0f);
        }
    }
    
    void setAmp(float newAmp){
        amp = newAmp;
    }
    
    void tempo(juce::AudioPlayHead* playhead)
    {
        reset();
        if (playhead == nullptr){
            return;
        }
        
        const auto opt = playhead->getPosition();
        if (!opt.hasValue()) {
            return;
        }
        
        const auto& pos = *opt;
        if (pos.getBpm().hasValue()) {
            bpm = *pos.getBpm();
        }
    }

    float generateWaveform()
    {
        phase = fastSin.sin(M_PI * 2.0f * phase);
        float waveform = pow(2.0f, (amp/10.0f) * phase);
        return waveform;
    }
        
private:
    juce::dsp::FastMathApproximations fastSin;
    float sampleRate;
    float phase = 0.0f;
    float amp = 0.0f;
    float accumHz = 0.0f;
    float accumSubdiv = 0.0f;
    float accumRatio = 0.0f;
    float bpm = 120.0f;
    
    std::array <double, 16> subdivisionMultiplier =
    {
        0.125,
        0.5 / 3.0,
        0.1875,
        0.25,
        1.0 / 3.0,
        0.375,
        0.5,
        2.0 / 3.0,
        0.75,
        1.0,
        4.0 / 3.0,
        1.5,
        2.0,
        8.0 / 3.0,
        3.0,
        4.0
    };
};
