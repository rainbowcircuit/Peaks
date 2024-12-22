#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "EarProtection.h"

//==============================================================================
PeaksAudioProcessor::PeaksAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

PeaksAudioProcessor::~PeaksAudioProcessor()
{
}

//==============================================================================
const juce::String PeaksAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PeaksAudioProcessor::acceptsMidi() const
{
    return true;
}

bool PeaksAudioProcessor::producesMidi() const
{
    return false;
}

bool PeaksAudioProcessor::isMidiEffect() const
{
    return false;
}

double PeaksAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================

int PeaksAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PeaksAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PeaksAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PeaksAudioProcessor::getProgramName (int index)
{
    return {};
}

void PeaksAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PeaksAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock);
    spec.numChannels = 2;
    
    for (int voice = 0; voice < 4; ++voice){
        // Peaks Reset
        peaksVoice[voice].prepareToPlay(getSampleRate(), getPlayHead());

        // voice allocation reset
        midiPitches[voice] = 0;
        midiVelocities[voice] = 0;
        activeVoices[voice] = false;
    }
    smoothedCutoff.reset(16);
    smoothedStructure.reset(16);
    voiceIndex = -1;
}

void PeaksAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PeaksAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void PeaksAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //==============================================================================

 //   juce::MidiBuffer::Iterator midiIterator(midiMessages);
    juce::MidiBuffer processedMIDI;
    
    for (const juce::MidiMessageMetadata metadata : midiMessages)
    {
        auto message = metadata.getMessage();
            
        if (message.isNoteOn()) {
            // index accum and reset
            voiceIndex += 1;
            if (voiceIndex >= 4){
                voiceIndex = 0;
            }
            for (int voice = 0; voice < 4; ++voice){
                if (!(midiPitches[voice] == message.getNoteNumber()) || !activeVoices[voice]){ // multiple voices shouldnt have the same pitch
                    midiPitches[voiceIndex] = message.getNoteNumber();
                    midiVelocities[voiceIndex] = message.getVelocity();
                    activeVoices[voiceIndex] = 1; // keep track of which note
                }
            }
        } else if (message.isNoteOff()) {
            for (int voice = 0; voice < 4; ++voice){
                if (midiPitches[voice] == message.getNoteNumber() && activeVoices[voice]){
                    midiVelocities[voice] = 0;
                    activeVoices[voice] = 0; // flag voice off
                }
            }
        }
        
        // debug only
            DBG("pitch1: " << midiPitches[0] << ", " << midiVelocities[0] <<
                "  pitch2: " << midiPitches[1] << ", " << midiVelocities[1] <<
                "  pitch3: " << midiPitches[2] << ", " << midiVelocities[2] <<
                "  pitch4: " << midiPitches[3] << ", " << midiVelocities[3]);
     }
    
    //==============================================================================

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    //==============================================================================
    // set parameters
    
    int partials = apvts.getRawParameterValue("partials")->load();
    float stereo = apvts.getRawParameterValue("stereo")->load();
    float cutoff = apvts.getRawParameterValue("frequency")->load();
    
    smoothedCutoff.setTargetValue(cutoff);
    float cutoffSmoothed = smoothedCutoff.getNextValue();

    float resonance = apvts.getRawParameterValue("resonance")->load();
    

    float structure = apvts.getRawParameterValue("structure")->load();
    smoothedStructure.setTargetValue(structure);
    float structSmoothed = smoothedStructure.getNextValue();

    float scale = apvts.getRawParameterValue("scale")->load();
    float offset = apvts.getRawParameterValue("offset")->load();
    float round = apvts.getRawParameterValue("round")->load();

    float lfoMode = apvts.getRawParameterValue("lfoMode")->load();
    float lfoInHz = apvts.getRawParameterValue("lfoInHz")->load();
    float lfoInBPM = apvts.getRawParameterValue("lfoInBPM")->load();
    float lfoInRatio = apvts.getRawParameterValue("lfoInRatio")->load();
    float lfoDepth = apvts.getRawParameterValue("lfoAmp")->load();
    float mix = apvts.getRawParameterValue("mix")->load();

    bool latch = apvts.getRawParameterValue("latch")->load();
    bool externalMIDI = apvts.getRawParameterValue("externalMIDI")->load();
    int transposition = apvts.getRawParameterValue("transposition")->load();

    
    // set values

    for (int voice = 0; voice < 4; ++voice){
        peaksVoice[voice].setRatioValues(scale, offset, round);
        peaksVoice[voice].setStructValues(structSmoothed);
        peaksVoice[voice].setFMValues(lfoMode, lfoInHz, lfoInBPM, lfoInRatio, lfoDepth);
    }

    // clear buffer
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    // voice processing
    //==============================================================================

    
    if (!externalMIDI){
        peaksVoice[0].setPeaksValues(partials, cutoffSmoothed, resonance, mix, stereo, latch);
        peaksVoice[0].setAmpValue(1.0f);
        peaksVoice[0].processBlock(buffer);
        
    } else if (externalMIDI){
        // setup accumulation buffer
        juce::AudioBuffer<float> accumBuffer;
        accumBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
        accumBuffer.clear();
        
        std::array<juce::AudioBuffer<float>, 4> voiceBuffer;
        for (int voice = 0; voice < 4; ++voice){
            voiceBuffer[voice].setSize(buffer.getNumChannels(), buffer.getNumSamples());
            voiceBuffer[voice].clear();
            
            // individual buffers for voices
            for(int channel = 0; channel < buffer.getNumChannels(); ++channel){
                voiceBuffer[voice].addFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples(), 1.0f);
            }
        }

        // process voices
        for (int voice = 0; voice < 4; ++voice){
            int transposedPitches = midiPitches[voice] + transposition;
            float frequences = juce::MidiMessage::getMidiNoteInHertz(transposedPitches);
            float amplitudes = midiVelocities[voice]/127.0f;
            
            peaksVoice[voice].setPeaksValues(partials, frequences, resonance, mix, stereo, latch);
            peaksVoice[voice].setAmpValue(smoothEnvelope[voice].processSample(amplitudes));
            peaksVoice[voice].processBlock(voiceBuffer[voice]);
            
            // accumulate
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel){
                accumBuffer.addFrom(channel, 0, voiceBuffer[voice], channel, 0, voiceBuffer[voice].getNumSamples(), 0.25f);
            }
            buffer.clear();
        }

        // add to original buffer
        for(int channel = 0; channel < buffer.getNumChannels(); ++channel){
            buffer.addFrom(channel, 0, accumBuffer, channel, 0, accumBuffer.getNumSamples(), 1.0f);
            buffer.applyGain(channel, 0, buffer.getNumSamples(), 0.25f);
        }
    }
    
    
#if JUCE_DEBUG
//    protectYourEars(buffer);
#endif
}

//==============================================================================
bool PeaksAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* PeaksAudioProcessor::createEditor()
{
    return new PeaksAudioProcessorEditor (*this, peaksVoice[0]);
}

//==============================================================================
void PeaksAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void PeaksAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement>xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PeaksAudioProcessor();
}

//==============================================================================


juce::AudioProcessorValueTreeState::ParameterLayout
    PeaksAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "mix", 1},
                                                            "Dry/Wet",
                                                            juce::NormalisableRange<float> { 0.0f, 100.0f, 0.01 }, 50.0f));
        
    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID { "partials", 1},
                                                            "Partials",
                                                            1, 8, 8));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "stereo", 1},
                                                            "Stereo Detune",
                                                            juce::NormalisableRange<float> { 0, 10.0f, 0.1f }, 0.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "frequency", 1},
                                                            "Frequency",
                                                            juce::NormalisableRange<float> { 20.0f, 1600.0f, 0.01f, 0.5f }, 220.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "resonance", 1},
                                                            "Resonance",
                                                            juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f, 0.5f }, 25.0f));
                    
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "structure", 1},
                                                            "Structure",
                                                            juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 100.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "scale", 1},
                                                            "Tuning Scaling",
                                                            juce::NormalisableRange<float> { 0.0f, 10.0f, 0.1f }, 5.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "offset", 1},
                                                            "Tuning Offset",
                                                            juce::NormalisableRange<float> { -30.0f, 30.0f, 0.1f }, 0.0f));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "round", 1},
                                                            "Tuning Rounding",
                                                            juce::NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.0f));
        
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { "latch", 1},
                                                            "Band Latch", false));
        
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "lfoMode", 1},
                                                            "LFO Mode",
                                                            juce::StringArray { "Hz", "BPM", "Ratio"}, 0));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "lfoInHz", 1},
                                                            "LFO Rate Hz",
                                                            juce::NormalisableRange<float> { 0.5f, 100.0f, 0.1f, 0.25 }, 0.0f));
        
    layout.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID { "lfoInBPM", 1},
                                                            "LFO Rate BPM",
                                                            juce::StringArray { "1/32", "1/16T", "1/32D", "1/16", "1/8T", "1/16D", "1/8",
                                                            "1/4T", "1/8D", "1/4", "1/2T", "1/4D", "1/2", "1/1T", "1/2D", "1/1" }, 9));
        
    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "lfoInRatio", 1},
                                                            "LFO Rate Ratio",
                                                            juce::NormalisableRange<float> { 0.25f, 15.0f, 0.125f }, 1.0f));


    layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { "lfoAmp", 1},
                                                            "LFO Amp",
                                                            juce::NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 0.0f));

    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { "externalMIDI", 1},
                                                            "External MIDI", false));

    layout.add(std::make_unique<juce::AudioParameterInt>(juce::ParameterID { "transposition", 1},
                                                            "Transposition",
                                                            -12, 12, 0));

    return layout;
}



