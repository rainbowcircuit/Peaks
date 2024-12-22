#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PeaksAudioProcessorEditor::PeaksAudioProcessorEditor (PeaksAudioProcessor& p, Peaks& q)
: AudioProcessorEditor (&p), mainGraphic(q.bandLevelMeasurements), audioProcessor (p)
{

    addAndMakeVisible(mainGraphic);
    addAndMakeVisible(lfoGraphic);
    
    //==============================================================================
    // Mix
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 15);
    mixSlider.setLookAndFeel(&mixGraphic);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "mix", mixSlider);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::right);
    mixLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 180));
    mixLabel.setFont(Fonts::getFont(13.0f));
    
    //==============================================================================
    // latch, stereo, and input
    addAndMakeVisible(latchButton);
    latchButton.setLookAndFeel(&latchGraphic);
    latchAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "latch", latchButton);
    
    addAndMakeVisible(stereoSlider);
    stereoSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    stereoSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    stereoSlider.setLookAndFeel(&stereoGraphic);
    stereoAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                           "stereo",
                                                                                              stereoSlider);

    addAndMakeVisible(inputButton);
    inputButton.setLookAndFeel(&inputGraphic);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "externalMIDI", inputButton);
    
    //==============================================================================
    // partials
    addAndMakeVisible(partialsSlider);
    partialsSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    partialsSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    partialsSlider.setLookAndFeel(&partialsGraphic);
    partialsAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                           "partials",
                                                                                           partialsSlider);
    
    addAndMakeVisible(partialsLabel);
    partialsLabel.setText("Partials", juce::dontSendNotification);
    partialsLabel.setJustificationType(juce::Justification::right);
    partialsLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 180));
    partialsLabel.setFont(Fonts::getFont(13.0f));
    
    // resonance
    addAndMakeVisible(resSlider);
    resSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    resSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
    resSlider.setLookAndFeel(&resGraphic);
    resSlider.setTextValueSuffix(" %");
    resAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                           "resonance",
                                                                                           resSlider);
    
    addAndMakeVisible(resLabel);
    resLabel.setText("Resonance", juce::dontSendNotification);
    resLabel.setJustificationType(juce::Justification::right);
    resLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 180));
    resLabel.setFont(Fonts::getFont(13.0f));

    // structure
    addAndMakeVisible(structSlider);
    structSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    structSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 15);
    structSlider.setLookAndFeel(&structGraphic);
    structSlider.setTextValueSuffix(" %");
    structAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                              "structure",
                                                                                              structSlider);
    
    addAndMakeVisible(structLabel);
    structLabel.setText("Structure", juce::dontSendNotification);
    structLabel.setJustificationType(juce::Justification::right);
    structLabel.setColour(juce::Label::textColourId, juce::Colour(180, 180, 180));
    structLabel.setFont(Fonts::getFont(13.0f));

    // frequency & transposition
    addAndMakeVisible(freqSlider);
    freqSlider.slider.setSkewFactor(0.5f);
    freqSlider.slider.setTextValueSuffix(" Hz");
    freqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                            "frequency",
                                                                                            freqSlider.slider);

    addAndMakeVisible(transLabel);
    transLabel.setText("Transposition", juce::dontSendNotification);
    transLabel.setJustificationType(juce::Justification::centred);
    transLabel.setColour(juce::Label::textColourId, juce::Colour(100, 99, 102));
    transLabel.setFont(Fonts::getFont(13.0f));

    addAndMakeVisible(transSlider);
    transSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    transSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    transSlider.setLookAndFeel(&transGraphic);
    transSlider.setTextValueSuffix(" st");
    transAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                            "transposition",
                                                                                             transSlider);

    //==============================================================================
    // scale, offset & round
    addAndMakeVisible(scaleSlider);
    scaleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                             "scale",
                                                                                             scaleSlider.slider);

    addAndMakeVisible(offsetSlider);
    offsetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                              "offset",
                                                                                              offsetSlider.slider);

    addAndMakeVisible(roundSlider);
    roundSlider.slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    roundSlider.slider.setMouseDragSensitivity(50);
    roundAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                             "round",
                                                                                             roundSlider.slider);
    
    //==============================================================================
    // LFO controls
    addAndMakeVisible(lfoRateHzSlider);
    lfoRateHzSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoRateHzSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    lfoRateHzSlider.setLookAndFeel(&lfoRateHzGraphic);
    lfoRateHzAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lfoInHz", lfoRateHzSlider);

    addAndMakeVisible(lfoRateBPMSlider);
    lfoRateBPMSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoRateBPMSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    lfoRateBPMSlider.setLookAndFeel(&lfoRateBPMGraphic);
    lfoRateBPMAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lfoInBPM", lfoRateBPMSlider);
    
    addAndMakeVisible(lfoRateRatioSlider);
    lfoRateRatioSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoRateRatioSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    lfoRateRatioSlider.setLookAndFeel(&lfoRatioGraphic);
    lfoRateRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lfoInRatio", lfoRateRatioSlider);
    
    addAndMakeVisible(lfoAmpSlider);
    lfoAmpSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoAmpSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    lfoAmpSlider.setLookAndFeel(&lfoAmpGraphic);
    lfoAmpAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lfoAmp", lfoAmpSlider);
    
    addAndMakeVisible(lfoModeSlider);
    lfoModeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lfoModeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    lfoModeSlider.setLookAndFeel(&lfoModeGraphic);
    lfoModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lfoMode", lfoModeSlider);
    
    addAndMakeVisible(rateLabel);
    rateLabel.setText("R:", juce::dontSendNotification);
    rateLabel.setJustificationType(juce::Justification::right);
    rateLabel.setColour(juce::Label::textColourId, juce::Colour(100, 99, 102));
    rateLabel.setFont(Fonts::getFont(12.0f));

    addAndMakeVisible(ampLabel);
    ampLabel.setText("A:", juce::dontSendNotification);
    ampLabel.setJustificationType(juce::Justification::right);
    ampLabel.setColour(juce::Label::textColourId, juce::Colour(100, 99, 102));
    ampLabel.setFont(Fonts::getFont(12.0f));
    
    //==============================================================================
    // add listener
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->addListener(this);
    }
    
    // set window size
    setSize (550, 240);
}

PeaksAudioProcessorEditor::~PeaksAudioProcessorEditor()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->removeListener(this);
    }
    setLookAndFeel(nullptr);
}

//==============================================================================
void PeaksAudioProcessorEditor::paint (juce::Graphics& g)
{
    // background color
    g.fillAll(Colours::editorBackground);

    // main graphics background color
    juce::Path maingraphicsPath, controlsPath, lfoPath;
    
    maingraphicsPath.addRoundedRectangle(183, 8, 358, 164, 6.0f);
    g.setColour(Colours::lightCream);
    g.fillPath(maingraphicsPath);

    g.setColour(Colours::lightCreamShadow);
    g.strokePath(maingraphicsPath, juce::PathStrokeType(1.5f));

    //==============================================================================
    controlsPath.addRoundedRectangle(183, 182, 358, 50, 4.0f);
    g.setColour(Colours::darkGrey);
    g.fillPath(controlsPath);
    g.setColour(Colours::darkGreyShadow);
    g.strokePath(controlsPath, juce::PathStrokeType(1.5f));

    lfoPath.addRoundedRectangle(8, 182, 165, 50, 4.0f);
    g.setColour(Colours::lightCream);
    g.fillPath(lfoPath);
    g.setColour(Colours::lightCreamShadow);
    g.strokePath(lfoPath, juce::PathStrokeType(1.5f));

    //==============================================================================
    // update main graphics
    float resonance = audioProcessor.apvts.getRawParameterValue("resonance")->load();
    float scale = audioProcessor.apvts.getRawParameterValue("scale")->load();
    float offset = audioProcessor.apvts.getRawParameterValue("offset")->load();
    float round = audioProcessor.apvts.getRawParameterValue("round")->load();
    float amp = audioProcessor.apvts.getRawParameterValue("structure")->load();
    float latch = audioProcessor.apvts.getRawParameterValue("latch")->load();
    int partials = audioProcessor.apvts.getRawParameterValue("partials")->load();
    
    mainGraphic.updateValues(partials, resonance, scale, offset, round, amp, latch);

    //==============================================================================

    float lfoMode = audioProcessor.apvts.getRawParameterValue("lfoMode")->load();
    float lfoRateInHz = audioProcessor.apvts.getRawParameterValue("lfoInHz")->load();
    int lfoRateInBPM = audioProcessor.apvts.getRawParameterValue("lfoInBPM")->load();
    int lfoRateInRatio = audioProcessor.apvts.getRawParameterValue("lfoInRatio")->load();
    float lfoAmp = audioProcessor.apvts.getRawParameterValue("lfoAmp")->load();

    lfoGraphic.updateValues(lfoMode, lfoRateInHz, lfoRateInBPM, lfoRateInRatio, lfoAmp);
    updateLFORateSliders(lfoMode);
    
    //==============================================================================

    int externalMIDI = audioProcessor.apvts.getRawParameterValue("externalMIDI")->load();
    updateFreqSliders(externalMIDI);

}

void PeaksAudioProcessorEditor::resized()
{
    // main graphic
    lfoGraphic.setBounds(8, 192, 157, 42);
    lfoGraphic.setInterceptsMouseClicks(false, false);
    
    mainGraphic.setBounds(175, 0, 358, 185);
    mainGraphic.setInterceptsMouseClicks(false, false);
    
    //==============================================================================
    // controls
    
    // latch, stereo and input
    latchButton.setBounds(18, 130, 30, 30);
    stereoSlider.setBounds(48, 130, 30, 30);
    inputButton.setBounds(78, 130, 30, 30);

    // mix and partials
    partialsSlider.setBounds(480, 208, 50, 15);
    partialsLabel.setBounds(435, 208, 50, 15);

    mixSlider.setBounds(480, 190, 60, 15);
    mixLabel.setBounds(435, 190, 50, 15);

    // resonance and structure
    resSlider.setBounds(315, 182, 120, 50);
    resLabel.setBounds(305, 190, 70, 15);
    
    structSlider.setBounds(185, 182, 120, 50);
    structLabel.setBounds(175, 190, 70, 15);

    // freq and transposition
    freqSlider.setTopLeftPosition(15, 19);
    transSlider.setBounds(15, 12, 100, 100);
    transLabel.setBounds(15, 105, 100, 15);
    
    // scale, offset and round
    scaleSlider.setTopLeftPosition(125, 15);
    offsetSlider.setTopLeftPosition(125, 75);
    roundSlider.setTopLeftPosition(125, 135);
    
    //==============================================================================
    // lfo
    lfoModeSlider.setBounds(12, 182, 50, 15);
    lfoRateHzSlider.setBounds(60, 182, 60, 15);
    lfoRateBPMSlider.setBounds(60, 182, 60, 15);
    lfoRateRatioSlider.setBounds(60, 182, 60, 15);
    rateLabel.setBounds(42, 182, 25, 15);
    
    lfoAmpSlider.setBounds(125, 182, 50, 15);
    ampLabel.setBounds(107, 182, 25, 15);
    repaint();
}

void PeaksAudioProcessorEditor::updateLFORateSliders(int lfoMode)
{
    lfoRateHzSlider.setVisible(lfoMode == 0);
    lfoRateBPMSlider.setVisible(lfoMode == 1);
    lfoRateRatioSlider.setVisible(lfoMode == 2);
}

void PeaksAudioProcessorEditor::updateFreqSliders(bool externalMIDI)
{
    freqSlider.setVisible(!externalMIDI);
    transSlider.setVisible(externalMIDI);
    transLabel.setVisible(externalMIDI);
}
