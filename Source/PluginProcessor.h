#pragma once

#include <JuceHeader.h>
#include "Peaks.h"
#include "Utility.h"
//==============================================================================

class PeaksAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    PeaksAudioProcessor();
    ~PeaksAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // parameter values
    juce::AudioProcessorValueTreeState apvts {
        *this, nullptr, "Parameters", createParameterLayout()
    };
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    int voiceIndex;
    std::array<Peaks, 4> peaksVoice;
    Peaks PeakMono;
    // voice allocation
    int const voiceCount = 4;
    std::array<float, 4> midiPitches;
    std::array<float, 4> midiVelocities;
    std::array<int, 4> activeVoices;
    
    std::array<EnvelopeSlew, 4> smoothEnvelope;
    
    juce::SmoothedValue<float> smoothedCutoff, smoothedStructure;
    /*
    enum VoiceState { Idle, Active, Release };
    VoiceState voiceStates[4] = { Idle };
    
     */
    //==============================================================================5
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeaksAudioProcessor)
};
