

#pragma once
#include <JuceHeader.h>

class PartialsGraphics2 : public juce::LookAndFeel_V4
{
public:

    PartialsGraphics2(){
        DBG("partials constructor called");
    }
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
    {
        auto boundsNoMargin = juce::Rectangle<int>(x, y, width, height).toFloat();
        auto bounds = boundsNoMargin.reduced(5.0f, 5.0f);
        
        
        //==============================================================================
        
        juce::Path graphicsPath, bgPath, onPath, offPath;
        
        //==============================================================================
        
        std::array<float, 2> dotIncrementSize = { bounds.getWidth()/5, bounds.getHeight()/7 };
        int displayVal = slider.getValue() - 1;

        for (int i = 0; i < 35; i++)
        {
            int j = std::floor(i/5);
            int k = i % 5;
            int radius = 1.5;
            
            
            if(displays[displayVal][i] == 1){
                onPath.addCentredArc(bounds.getX() + k * dotIncrementSize[0],
                                    bounds.getY() + j * dotIncrementSize[1],
                                    radius, radius, 0.0f, 0.0f, 6.28f, true);
                g.setColour(juce::Colour(243, 203, 99));
                g.fillPath(onPath);
                
                
            } else if(displays[displayVal][i] == 0){
                offPath.addCentredArc(bounds.getX() + k * dotIncrementSize[0],
                                    bounds.getY() + j * dotIncrementSize[1],
                                    radius, radius, 0.0f, 0.0f, 6.28f, true);
                g.setColour(juce::Colour(105, 105, 99));
                g.fillPath(offPath);

            }
        }
        
        /*
        if (slider.isMouseOverOrDragging()){
            slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
            g.setColour(juce::Colour(243, 203, 99));
            
        } else {
            slider.setMouseCursor(juce::MouseCursor::NormalCursor);
            g.setColour(juce::Colour(233, 193, 89));
        }
        g.fillPath(graphicsPath);
         */
    }
    

private:

//==============================================================================
// cell values
    std::array<std::array<int, 35>, 8> displays = {{
        //display1
        {0, 0, 1, 0, 0,
            0, 1, 1, 0, 0,
            1, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            0, 0, 1, 0, 0,
            1, 1, 1, 1, 1},
        //display2
        {0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 0, 0,
            1, 1, 1, 1, 1},
        //display3
        {0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            0, 0, 1, 1, 0,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0},
        //display4
        {0, 0, 0, 1, 0,
            0, 0, 1, 1, 0,
            0, 1, 0, 1, 0,
            1, 0, 0, 1, 0,
            1, 1, 1, 1, 1,
            0, 0, 0, 1, 0,
            0, 0, 0, 1, 0},
        //display5
        {1, 1, 1, 1, 1,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 0,
            0, 0, 0, 0, 1,
            0, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0},
        //display6
        {0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 0,
            1, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0},
        //display7
        {1, 1, 1, 1, 1,
            0, 0, 0, 0, 1,
            0, 0, 0, 1, 0,
            0, 0, 1, 0, 0,
            0, 1, 0, 0, 0,
            0, 1, 0, 0, 0,
            0, 1, 0, 0, 0},
        //display8
        {0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0,
            1, 0, 0, 0, 1,
            1, 0, 0, 0, 1,
            0, 1, 1, 1, 0}
    }};



};
