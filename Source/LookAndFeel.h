/*
  ==============================================================================

    LookAndFeel.h
    Created: 23 Oct 2024 10:34:33am
    Author:  Takuma Matsui

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Colours
{
    const juce::Colour editorBackground { 238, 234, 222 };
    const juce::Colour lightCream { 228, 224, 212 };
    const juce::Colour lightCreamShadow { 223, 219, 207 };

    const juce::Colour baseYellow { 233, 193, 89 };
    const juce::Colour baseYellowHover { 243, 203, 99 };

    const juce::Colour highlightWhite { 238, 234, 222 };
    const juce::Colour darkGrey { 100, 99, 102 };
    const juce::Colour darkGreyHover { 110, 109, 112 };

    const juce::Colour darkGreyShadow { 95, 94, 97 };
    const juce::Colour lightGrey { 200, 199, 202};
    const juce::Colour lightGreyShadow { 180, 179, 182};
}


class Fonts
{
public:
    Fonts() = delete;
    static juce::Font getFont(float height = 16.0f);
    
private:
    static const juce::Typeface::Ptr typeface;
};
