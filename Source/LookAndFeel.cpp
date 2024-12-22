/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 23 Oct 2024 10:34:33am
    Author:  Takuma Matsui

  ==============================================================================
*/

#include "LookAndFeel.h"

const juce::Typeface::Ptr Fonts::typeface = juce::Typeface::createSystemTypefaceFor(BinaryData::LatoMedium_ttf, BinaryData::LatoMedium_ttfSize);

juce::Font Fonts::getFont(float height)
{
    return juce::FontOptions(typeface).withHeight(height);
}
