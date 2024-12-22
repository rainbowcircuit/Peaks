#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Peaks.h"

#include "DisplayGraphics.h"
#include "UserInterfaceGraphics.h"
#include "MiscGraphics.h"
#include "LookAndFeel.h"
//==============================================================================

class PeaksAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::AudioProcessorParameter::Listener
{
public:
    PeaksAudioProcessorEditor (PeaksAudioProcessor&, Peaks& q);
    ~PeaksAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    //==============================================================================

private:
    void parameterValueChanged(int, float) override {}
    void parameterGestureChanged(int, bool) override {}
    //==============================================================================
    void updateLFORateSliders(int lfoMode);
    void updateFreqSliders(bool externalMIDI);

    
    // slidersAndParameters
    int lfoMode;
    
    //==============================================================================
    // look and feel is instantiated first
    TextDisplaySliderGraphics
        lfoModeGraphic { "", 11.0f, juce::Colour(100, 99, 102), juce::Justification::left },
        lfoRateHzGraphic { " Hz", 11.0f, juce::Colour(100, 99, 102), juce::Justification::left},
        lfoRateBPMGraphic { "", 11.0f, juce::Colour(100, 99, 102), juce::Justification::left },
        lfoRatioGraphic { ":1", 11.0f, juce::Colour(100, 99, 102), juce::Justification::left },
        lfoAmpGraphic { " %", 11.0f, juce::Colour(100, 99, 102), juce::Justification::left },
        mixGraphic { " %", 12.0f, juce::Colour(238, 234, 222), juce::Justification::left },
        partialsGraphic { "", 12.0f, juce::Colour(238, 234, 222), juce::Justification::left };

    MainGraphics mainGraphic;
    LFOGraphics lfoGraphic;
    OrangeDialGraphics
        freqGraphic { false }, transGraphic { true };
    GreyDialGraphics dialGraphic;
    RoundGraphics roundGraphic;
    ResonanceGraphics resGraphic;
    StructureGraphics structGraphic;
    LatchGraphics latchGraphic;
    InputGraphics inputGraphic;
    StereoGraphics stereoGraphic;
    
    //==============================================================================
    // followed by actual sliders or buttons
    juce::Slider
        mixSlider, partialsSlider, resSlider, structSlider, transSlider,
        lfoModeSlider, lfoRateHzSlider, lfoRateBPMSlider, lfoRateRatioSlider, lfoAmpSlider,
        stereoSlider;
    
    juce::ToggleButton
        latchButton, inputButton, lfoHzButton, lfoBPMButton, lfoRatioButton;
    
    juce::Label
        peaksLabel, transLabel, resLabel, structLabel, mixLabel, partialsLabel, ampLabel, rateLabel;
    
    GUIGraphics
        freqSlider { 100, 100, "Frequency", freqGraphic },
        scaleSlider { 40, 55, "Scale", dialGraphic },
        offsetSlider { 40, 55, "Offset", dialGraphic },
        roundSlider { 40, 35, "Round", roundGraphic };
    
    //==============================================================================
    // finally attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
        mixAttachment, posAttachment, partialsAttachment, freqAttachment, transAttachment,
        resAttachment, structAttachment, scaleAttachment, offsetAttachment, roundAttachment,
        lfoModeAttachment, lfoRateHzAttachment, lfoRateBPMAttachment, lfoRateRatioAttachment,
        lfoAmpAttachment, stereoAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
        latchAttachment, inputAttachment;

    PeaksAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PeaksAudioProcessorEditor)
};

