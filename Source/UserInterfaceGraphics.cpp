/*
  ==============================================================================

    GUIGraphics.cpp
    Created: 21 Dec 2024 11:08:00pm
    Author:  Takuma Matsui

  ==============================================================================
*/
#include <JuceHeader.h>
#include "UserInterfaceGraphics.h"

GUIGraphics::GUIGraphics(int width, int height,
                   const juce::String& paramName, juce::LookAndFeel_V4& newLookAndFeel) : width(width), height(height), lookAndFeel(newLookAndFeel)
{
    // label
    addAndMakeVisible(nameLabel);
    nameLabel.setText(paramName, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::horizontallyCentred);
    juce::Font nameLabelFont(Fonts::getFont(13.0f));
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
    nameLabel.setColour(juce::Label::textColourId, Colours::darkGrey);
    nameLabel.setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);

    timerCallback();
    setSize(width, height + 15); // 120 graphic + 15 slider
}


GUIGraphics::~GUIGraphics()
{
    slider.removeListener(this);
    slider.setLookAndFeel(nullptr);
}

//==============================================================================

void GUIGraphics::resized()
{
    nameLabel.setBounds(0, height - 15, width, 15);
    slider.setBounds(0, 0, width, height);
}

//==============================================================================

void GUIGraphics::sliderValueChanged(juce::Slider* slider)
{
    startTimer(500);

    slider->setColour(juce::Slider::textBoxTextColourId, Colours::darkGrey);
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    nameLabel.setColour(juce::Label::textColourId, juce::Colours::transparentBlack);
    repaint();
}

void GUIGraphics::timerCallback()
{
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    nameLabel.setColour(juce::Label::textColourId, Colours::darkGrey);
    
    repaint();
    stopTimer();
}

//==============================================================================

StructureGraphics::StructureGraphics()
{
    setColour(juce::Slider::textBoxTextColourId, Colours::highlightWhite);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);

}

void StructureGraphics::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
        
    juce::Path graphicPath, graphicLeftPath, graphicRightPath, graphicRightBackPath, highlightPath;
    
    //==============================================================================
    float isoSkewFactor = 2.0f;
    float boxWidth = bounds.getHeight() * 1.5f;
    float boxHeight = bounds.getHeight() * 0.5f;
        
    float rotationFactor = sliderPosProportional * 1.57f;
    float topHeightOffset = bounds.getCentreY() - boxHeight/2;
    float botHeightOffset = bounds.getCentreY() + boxHeight;
        

    juce::Point topLeftCoords = { bounds.getCentreX() - (cos(rotationFactor) * boxWidth/2),
    topHeightOffset -  (sin(rotationFactor) * boxHeight/2/isoSkewFactor) };
    juce::Point topCentreBotCoords = { bounds.getCentreX() - (cos(rotationFactor - 1.57f) * boxWidth/2),
    topHeightOffset -  (sin(rotationFactor - 1.57f) * boxHeight/2/isoSkewFactor) };
    juce::Point topRightCoords = { bounds.getCentreX() + (cos(rotationFactor) * boxWidth/2),
    topHeightOffset + (sin(rotationFactor) * boxHeight/2/isoSkewFactor) };
    juce::Point topCentreTopCoords = { bounds.getCentreX() + (cos(rotationFactor - 1.57f) * boxWidth/2),
    topHeightOffset +  (sin(rotationFactor - 1.57f) * boxHeight/2/isoSkewFactor) };

    juce::Point botLeftCoords = { bounds.getCentreX() - boxWidth/2, botHeightOffset - (boxHeight/2) };
    juce::Point botCentreBotCoords = { bounds.getCentreX(), botHeightOffset - (boxHeight/2/isoSkewFactor) };
    juce::Point botRightCoords = { bounds.getCentreX() + boxWidth/2, botHeightOffset - (boxHeight/2) };
    juce::Point botCentreTopCoords = { bounds.getCentreX(), botHeightOffset - (boxHeight/2/isoSkewFactor) };
        
    //==============================================================================

    juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
        
    //==============================================================================
    // left panel
    graphicLeftPath.startNewSubPath(botLeftCoords);
    graphicLeftPath.lineTo(botCentreBotCoords);
    graphicLeftPath.lineTo(topCentreBotCoords);
    graphicLeftPath.lineTo(topLeftCoords);
    graphicLeftPath.closeSubPath();
    graphicLeftPath = graphicLeftPath.createPathWithRoundedCorners(2.0f);

    g.setColour(Colours::highlightWhite);
    g.fillPath(graphicLeftPath);
    g.strokePath(graphicLeftPath, strokeType);
        
    // right Panel
    graphicRightPath.startNewSubPath(botRightCoords);
    graphicRightPath.lineTo(botCentreBotCoords);
    graphicRightPath.lineTo(topCentreBotCoords);
    graphicRightPath.lineTo(topRightCoords);
    graphicRightPath.closeSubPath();
    graphicRightPath = graphicRightPath.createPathWithRoundedCorners(2.0f);

    g.setColour(Colours::darkGrey);
    g.fillPath(graphicRightPath);
    g.strokePath(graphicRightPath, strokeType);

    
    // highlight panel
    highlightPath.startNewSubPath(topRightCoords);
    highlightPath.lineTo(topCentreBotCoords);
    highlightPath.lineTo(botCentreBotCoords);
    highlightPath.closeSubPath();
    graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
    g.setColour(juce::Colour(255, 255, 255).withAlpha(sliderPosProportional * 0.15f));
    g.fillPath(highlightPath);
    g.strokePath(highlightPath, strokeType);

    // top panel
    graphicPath.startNewSubPath(topLeftCoords);
    graphicPath.lineTo(topCentreTopCoords);
    graphicPath.lineTo(topRightCoords);
    graphicPath.lineTo(topCentreBotCoords);
    graphicPath.closeSubPath();
    graphicPath = graphicPath.createPathWithRoundedCorners(2.0f);
    
    if (slider.isMouseOverOrDragging()){
        slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        g.setColour(Colours::baseYellowHover);
    } else {
        slider.setMouseCursor(juce::MouseCursor::NormalCursor);
        g.setColour(Colours::baseYellow);
    }
    
    g.fillPath(graphicPath);
    g.strokePath(graphicPath, strokeType);
    
}

juce::Slider::SliderLayout StructureGraphics::getSliderLayout(juce::Slider& slider)
{
    juce::Slider::SliderLayout layout;
    layout.textBoxBounds = juce::Rectangle<int> (0, 25, 60, 15);
    layout.sliderBounds = juce::Rectangle<int> (60, 0, 70, 50);
    
    return layout;
}

juce::Label* StructureGraphics::createSliderTextBox(juce::Slider& slider)
{
    auto label = LookAndFeel_V4::createSliderTextBox(slider);
    
    label->setFont(Fonts::getFont(13.0f));
    label->setJustificationType(juce::Justification::right);
    

    label->onEditorShow = [label]()
    {
        if(auto* editor = label->getCurrentTextEditor())
        {
            editor->setJustification(juce::Justification::centred);
        }
        
    };
    return label;
}

//==============================================================================

ResonanceGraphics::ResonanceGraphics()
{
    setColour(juce::Slider::textBoxTextColourId, Colours::highlightWhite);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
}

void ResonanceGraphics::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
        
    juce::Path graphicPath, bgPath;
    //==============================================================================
    
    float isoSkewFactor = 2.5f;
    
    float graphicHeight = bounds.getHeight() * 0.85f;
    juce::PathStrokeType strokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

   for (int i = 0; i < 6; i++){
       float bgIncrement = i * (bounds.getWidth()/12);
       bgPath.startNewSubPath((bounds.getCentreX() - bounds.getWidth()/2) + bgIncrement, (bounds.getCentreY() - graphicHeight/4) - (bgIncrement/isoSkewFactor));
       bgPath.lineTo(bounds.getCentreX() + bgIncrement, (bounds.getCentreY() + graphicHeight/2) - (bgIncrement/isoSkewFactor));

       g.setColour(Colours::highlightWhite.withAlpha(0.15f));
       g.strokePath(bgPath, strokeType);
       
       //==============================================================================
       
       float fgIncrement = (sliderPosProportional * 0.75f + 0.25f) * i * (bounds.getWidth()/12);
       graphicPath.startNewSubPath((bounds.getCentreX() - bounds.getWidth()/2) + fgIncrement, (bounds.getCentreY() - graphicHeight/4) - (fgIncrement/isoSkewFactor));
       graphicPath.lineTo(bounds.getCentreX() + fgIncrement, (bounds.getCentreY() + graphicHeight/2) - (fgIncrement/isoSkewFactor));
       
       if (slider.isMouseOverOrDragging()){
           slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
           g.setColour(Colours::baseYellowHover);
       } else {
           slider.setMouseCursor(juce::MouseCursor::NormalCursor);
           g.setColour(Colours::baseYellow);
       }
       g.strokePath(graphicPath, strokeType);
    }
}
    
juce::Slider::SliderLayout ResonanceGraphics::getSliderLayout(juce::Slider& slider)
{
    juce::Slider::SliderLayout layout;
    layout.textBoxBounds = juce::Rectangle<int> (0, 25, 60, 15);
    layout.sliderBounds = juce::Rectangle<int> (60, 0, 60, 50);
    
    return layout;
}

juce::Label* ResonanceGraphics::createSliderTextBox(juce::Slider& slider)
{
    auto label = LookAndFeel_V4::createSliderTextBox(slider);
    label->setFont(Fonts::getFont(13.0f));
    label->setJustificationType(juce::Justification::right);
    

    label->onEditorShow = [label]()
    {
        if(auto* editor = label->getCurrentTextEditor())
        {
            editor->setJustification(juce::Justification::centred);
        }
    };
    return label;
}

void GreyDialGraphics::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                      float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto bounds = boundsNoMargin.reduced(2.0f, 2.0f);
    
    //==============================================================================

    float pi = juce::MathConstants<float>::pi;
    float dialStart = 1.25f * pi;
    float dialEnd = 2.75f * pi;
    float sliderPositionScaled = 2.0f + (1.0f - sliderPosProportional);
    float dialPositionInRadians = dialStart + sliderPositionScaled * (dialEnd - dialStart);
    
    //==============================================================================

    juce::Path dialBodyPath, dialDotPath, dialOutlinePath;
    
    //==============================================================================

    if (slider.isMouseOverOrDragging()){
        slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        g.setColour(Colours::darkGreyHover);
    } else {
        slider.setMouseCursor(juce::MouseCursor::NormalCursor);
        g.setColour(Colours::darkGrey);
    }

    // dial outline
    float dialOutlineRadius = (bounds.getWidth())/2;
    float dialBodyRadius = (bounds.getWidth() * 0.85f)/2;
    float dialDotRadius = (bounds.getWidth() * 0.6f)/2;

    dialOutlinePath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                  dialOutlineRadius, dialOutlineRadius,
                                  0.0f, dialStart, dialEnd, true);
    
    g.strokePath(dialOutlinePath, juce::PathStrokeType(1.0f));

    //==============================================================================
    // dial body
    dialBodyPath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                               dialBodyRadius, dialBodyRadius,
                               0.0f, 0.0f, 6.28f, true);
    
    g.fillPath(dialBodyPath);
    
    //==============================================================================
    // dial dot
    std::array<float, 2> needleCoords = {bounds.getCentreX() + std::sin(dialPositionInRadians) * dialDotRadius,
                                        bounds.getCentreX() + std::cos(dialPositionInRadians) * dialDotRadius};

    dialDotPath.addCentredArc(needleCoords[0], needleCoords[1],
                              1.5, 1.5, 0.0f, 0.0f, pi * 2, true);
    
    g.setColour(juce::Colour(255, 255, 255)); //
    g.fillPath(dialDotPath);
    
}

void GreyDialGraphics::fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    g.setColour(Colours::darkGrey);
    
    juce::Path bgPath;
    bgPath.addRoundedRectangle(textEditor.getLocalBounds().reduced(4.0f, 0.0f), 3.0f);
    g.fillPath(bgPath);
}

juce::Label* GreyDialGraphics::createSliderTextBox(juce::Slider& slider)
{
    auto label = LookAndFeel_V4::createSliderTextBox(slider);
    
    label->setFont(Fonts::getFont(13.0f));
    label->setJustificationType(juce::Justification::centred);
    

    label->onEditorShow = [label]()
    {
        if(auto* editor = label->getCurrentTextEditor())
        {
            editor->setJustification(juce::Justification::centred);
        }
        
    };
    return label;
}

void OrangeDialGraphics::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto bounds = boundsNoMargin.reduced(10.0f, 10.0f);
    
    float pi = juce::MathConstants<float>::pi;
    float dialStart = 1.25f * pi;
    float dialEnd = 2.75f * pi;
    float dialPositionInRadians = dialStart + sliderPosProportional * (dialEnd - dialStart);

    float dialRadius = bounds.getWidth() * 0.425;

    
    juce::Path sliderValuePath, sliderBGPath;
    juce::PathStrokeType strokeType(8.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

    //==============================================================================
    // bg path
    sliderBGPath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                               dialRadius, dialRadius,
                               0.0f, dialStart, dialEnd, true);
    g.setColour(Colours::darkGrey);
    g.strokePath(sliderBGPath, strokeType);

    //==============================================================================
    // value path
    sliderValuePath.addCentredArc(bounds.getCentreX(), bounds.getCentreY(),
                                  dialRadius, dialRadius,
                                 0.0f, dialStart, dialPositionInRadians, true);
    
    if (slider.isMouseOverOrDragging()){
        slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
        g.setColour(Colours::baseYellowHover);
    } else {
        slider.setMouseCursor(juce::MouseCursor::NormalCursor);
        g.setColour(Colours::baseYellow);
    }
    
    g.strokePath(sliderValuePath, strokeType);

    //==============================================================================
    // text
    
    if (!externalMIDI){
        int midiNote = log(slider.getValue()/440.0)/log(2) * 12 + 69;
        
        juce::String display = juce::MidiMessage::getMidiNoteName(midiNote, false, true, 4);
        int noteCents = (slider.getValue() - (int)slider.getValue()) * 100;
        
        g.setColour(Colours::darkGrey);
        g.setFont(Fonts::getFont(20.0f));
        g.drawText(display, bounds.getX(), bounds.getY() - 4, bounds.getWidth(), bounds.getHeight(), juce::Justification::centred);
        
        g.setFont(bounds.getWidth() * 0.15f);
        g.drawText("+" + juce::String(noteCents) + " ct", bounds.getCentreX() - 20,
                   bounds.getCentreY() + 2, 40,
                   20, juce::Justification::centred);
    } else {
        juce::String display = juce::String(slider.getValue()) + " st";
        
        g.setColour(Colours::darkGrey);
        g.setFont(Fonts::getFont(13.0f));
        g.drawText(display, bounds, juce::Justification::centred, false);
    }
}

void OrangeDialGraphics::fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    g.setColour(Colours::darkGrey);
    juce::Path bgPath;
    bgPath.addRoundedRectangle(textEditor.getLocalBounds().reduced(4.0f, 0.0f), 3.0f);
    g.fillPath(bgPath);
}

juce::Label* OrangeDialGraphics::createSliderTextBox(juce::Slider& slider)
{
    auto label = LookAndFeel_V4::createSliderTextBox(slider);
    
    label->setFont(Fonts::getFont(13.0f));
    label->setJustificationType(juce::Justification::centred);
    

    label->onEditorShow = [label]()
    {
        if(auto* editor = label->getCurrentTextEditor())
        {
            editor->setJustification(juce::Justification::centred);
        }
    };
    return label;
}


