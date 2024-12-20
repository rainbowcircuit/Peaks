#include <JuceHeader.h>
#include "Graphics.h"
#include "Utility.h"
#include "Structure.h"
//==============================================================================
Graphics::Graphics(int width, int height,
                   const juce::String& paramName, juce::LookAndFeel_V4& newLookAndFeel) : width(width), height(height), lookAndFeel(newLookAndFeel)
{
    // label
    addAndMakeVisible(nameLabel);
    nameLabel.setText(paramName, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::horizontallyCentred);
    juce::Font nameLabelFont(12.0f, juce::Font::plain);
    nameLabel.setFont(nameLabelFont);
    nameLabel.setBorderSize(juce::BorderSize<int> (0));
    
    // slider
    addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, width, 15);
    slider.setLookAndFeel(&newLookAndFeel);
    slider.addListener(this);
    
    // default colours
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    nameLabel.setColour(juce::Label::textColourId, juce::Colour(100, 99, 102));
    nameLabel.setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

    timerCallback();
    setSize(width, height + 15); // 120 graphic + 15 slider
}


Graphics::~Graphics()
{
    slider.removeListener(this);
    slider.setLookAndFeel(nullptr);
 //   setLookAndFeel(nullptr);
 //   nameLabel.setLookAndFeel(nullptr);
    

}

//==============================================================================

void Graphics::paint (juce::Graphics& g)
{
 //   g.setColour(juce::Colours::lightblue);
 //   g.drawRect(getLocalBounds(), 2);
}

void Graphics::resized()
{
    nameLabel.setBounds(0, height - 15, width, 15);
    slider.setBounds(0, 0, width, height);
}

//==============================================================================

void Graphics::mouseEnter(const juce::MouseEvent& event)
{    
    repaint();
}
void Graphics::mouseExit(const juce::MouseEvent& event)
{
    repaint();

}
//==============================================================================

void Graphics::sliderValueChanged(juce::Slider* slider)
{
    startTimer(500);

    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(100, 99, 102));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    nameLabel.setColour(juce::Label::textColourId, juce::Colours::transparentBlack);
    
    repaint();
}

void Graphics::timerCallback()
{
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    nameLabel.setColour(juce::Label::textColourId, juce::Colour(100, 99, 102));
    
    repaint();
    stopTimer();
}



//==============================================================================



LFOGraphics::LFOGraphics()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->addListener(this);
    }
}

LFOGraphics::~LFOGraphics()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->removeListener(this);
    }
}

void LFOGraphics::paint(juce::Graphics& g)
{
    auto rectNoBounds = getLocalBounds().toFloat();
    auto bounds = rectNoBounds.reduced(10.0f, 10.0f);
    //==============================================================================
    juce::Path graphicsPath, gridlinePath;
    
    //==============================================================================
    // dotted line
    juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    float dashLengths[] = { 0.75f, 6.0f };
    
    gridlinePath.startNewSubPath(bounds.getX(), bounds.getCentreY());
    gridlinePath.lineTo(bounds.getX() + bounds.getWidth(), bounds.getCentreY());
    strokeType.createDashedStroke(gridlinePath, gridlinePath, dashLengths, 2);

    g.setColour(juce::Colour(150, 150, 150));
   // g.strokePath(gridlinePath, strokeType);

    
    //==============================================================================
    // waveform
    
    float twoPi = 2.0f * juce::MathConstants<float>::pi;
    graphicsPath.startNewSubPath(bounds.getX(), bounds.getCentreY());
        
 //   DBG(lfoAmp);
    for (int i = 1; i < bounds.getWidth() - 2; i++){
        
        float waveform = (lfoAmp/100.0f) * sin((twoPi * (lfoRate * 20.0f + 5.0f) * i/bounds.getWidth()));
        graphicsPath.lineTo(bounds.getX() + i * 2, bounds.getCentreY() + waveform * (bounds.getHeight()/2));
        
    }
    graphicsPath = graphicsPath.createPathWithRoundedCorners(2.0f);
    juce::PathStrokeType waveformStroke(2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    g.setColour(juce::Colour(233, 193, 89));
    g.strokePath(graphicsPath, waveformStroke);
    
}

void LFOGraphics::updateValues(float newMode, float newRateInHz, float newRateInBPM, float newRateInRatio, float newAmp)
{
    
    lfoRate = newRateInHz/100.0f;
    
    if (newMode == 1){
        lfoRate = newRateInBPM/16.0f;
        
    } else if (newMode== 2){
        lfoRate = newRateInRatio/15.0f;
        
    }
    
    lfoAmp = newAmp;
    lfoMode = newMode;
    repaint();
}

void LFOGraphics::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}


//==============================================================================




















