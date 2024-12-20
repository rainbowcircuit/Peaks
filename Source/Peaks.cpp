
#include "Peaks.h"
#include <math.h>

void Peaks::prepareToPlay(float sampleRate, juce::AudioPlayHead* playhead)
{
    for(int i = 0; i < 2; i++){
        svfLP[i].reset();
        svfLP[i].setSampleRate(sampleRate);
        combDelay[i].setMaximumDelayInSamples(sampleRate);
        combDelay[i].reset();
    }
    lfo.tempo(playhead);
    lfo.reset();
    lfo.setSampleRate(sampleRate);

    for(int i = 0; i < 8; i++){
        // resets SVF
        svfBP_L[i].reset();
        svfBP_L[i].setSampleRate(sampleRate);
        svfBP_R[i].reset();
        svfBP_R[i].setSampleRate(sampleRate);
    }
}

//==============================================================================

void Peaks::setPeaksValues(int newPartials, float newCutoff, float newResonance, float newMix, float newStereo, bool newLatch)
{
    partials = newPartials;
    cutoff = newCutoff;
    resonance = newResonance;
    mix = newMix;
    stereo = newStereo;
    latch = newLatch;
}


void Peaks::setRatioValues(float newScale, float newOffset, float newRound)
{
    
    std::array<float, 8> ratioValInt;
    std::array<float, 8> ratioValFloat;

    float scaleScaled = (newScale * 0.1f) + 1.0f;
    for (int i = 0; i < 8; i++) {
        ratioValFloat[i] = std::pow(i + 1, scaleScaled) + newOffset;
        
        // mode
        if (latch){
            ratioValFloat[0] = 1.0f;
            ratioValInt[0] = 1.0f;
        }
        // nyquist safety
        if (ratioValFloat[i] * cutoff >= 22000.0f) {
            ratioValFloat[i] = 22000.0f / cutoff;
        } else if (ratioValFloat[i] <= 1.0f) {
            ratioValFloat[i] = 1.0f;
        }
        
        
        ratioValInt[i] = (int)ratioValFloat[i];
        ratioVal[i] = (ratioValFloat[i] * (1.0 - newRound)) + (ratioValInt[i] * newRound);
    }
    
}
void Peaks::setAmpValue(float newAmplitude){
    amplitude = newAmplitude;
}

void Peaks::setStiffnessValues(float newPosition, float newStructure)
{
    position = newPosition;
    
    float structScaled = newStructure/100.0f + 0.15f;
    for (int i = 0; i < 8; i++){
        ampVal[i] = 1.0f - abs(sin((i + 1) * structScaled));
        if (latch){
            ampVal[0] = 0.8f; // 1.0f looks a bit too intense
        }
    }
}


//==============================================================================

float Peaks::positionProcessSample(float input, float amount, int channel) noexcept
{
    float filterCutoff = abs(amount - 50.0f);

    svfLP[channel].setCoefficients(filterCutoff * 150 + 12500, 5.0f);
    float filterOut = svfLP[channel].processSample(input, 1);
    combDelay[channel].write(filterOut);

    float delayTime = amount * 10.22 + 1;
    float delayOut = combDelay[channel].read(delayTime);

    float positionOut = (delayOut * (1.0f - (amount / 100.0f))) + input;
    return positionOut;
}


float Peaks::softClipProcessSample (float input) noexcept
{
    return tanhf(input * 3.0f);
}

void Peaks::setFMValues(int newMode, float newRateInHz, float newRateInBPM, float newRateInRatio, float newAmp)
{
    lfoMode = newMode;
    lfoRateInHz = newRateInHz;
    lfoRateInBPM = newRateInBPM;
    lfoRateInRatio = newRateInRatio;
    lfoAmp = newAmp;
}


void Peaks::processBlock(juce::AudioBuffer<float>& buffer) noexcept
{
    float resonanceScaled = 100.0f + (resonance * 39.0f); // 100 ~ 1500 range
    float stereoScaled = stereo / 250.0f;
    
    // set up write pointers
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);
    
    // processing
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample){
        // lfo handling
        lfo.setTimeStep(lfoMode, lfoRateInHz, lfoRateInBPM, cutoff, lfoRateInRatio);
        lfo.setAmp(lfoAmp);
        float lfoWaveform = lfo.generateWaveform();

        float inputL = channelDataL[sample] * amplitude;
        float inputR = channelDataR[sample] * amplitude;
        float WetL = 0.0f;
        float WetR = 0.0f;

        for(int i = 0; i < partials; i++){
            
            svfBP_L[i].setCoefficients((cutoff + lfoWaveform) * (ratioVal[i] + stereoScaled), resonanceScaled);
            svfBP_R[i].setCoefficients((cutoff + lfoWaveform) * (ratioVal[i]), resonanceScaled);

            float bandL = svfBP_L[i].processSample(inputL, 2) * (sqrt(resonanceScaled));
            float bandOutL = bandL * ampVal[i];
            float bandR = svfBP_R[i].processSample(inputR, 2) * (sqrt(resonanceScaled));
            float bandOutR = bandR * ampVal[i];
                
            WetL += bandOutL;
            WetR += bandOutR;
        }
        
        float wetOutputL = outputDCBlockL.processSample(softClipProcessSample(WetL));
        float dryOutputL = channelDataL[sample] * amplitude;
        float wetOutputR = outputDCBlockR.processSample(softClipProcessSample(WetR));
        float dryOutputR = channelDataR[sample] * amplitude;

        // write back to buffer
        float mixScaled = mix/100.0f;
        channelDataL[sample] = dryOutputL * (1.0f - mixScaled) + wetOutputL * mixScaled;
        channelDataR[sample] = dryOutputR * (1.0f - mixScaled) + wetOutputR * mixScaled;
        
    }
}


