
#include "MainGraphics.h"

MainGraphics::MainGraphics(std::array<std::atomic<float>, 8>& _bandLevel) : _bandLevel(_bandLevel)
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->addListener(this);
    }
    ratioValues();
}

MainGraphics::~MainGraphics()
{
    const auto& params = audioProcessor.getParameters();
    for (auto param : params){
        param->removeListener(this);
    }
}


//==============================================================================


void MainGraphics::paint(juce::Graphics& g)
{
    
    juce::Path visualizerPath, visualizerOffPath, boundsFill;
    
    auto rectNoBounds = getLocalBounds().toFloat();
    auto bounds = rectNoBounds.reduced(8.0f, 8.0f);

    
    for (int i = 0; i < 8; ++i){
        float peakWidthScaling = bounds.getWidth() * 0.6f;
        float peakCurveOffset = bounds.getWidth() * (0.025 + (0.025 * (1.0f - resonance/100.0f))); 
        float peakCenterLevel = peakLevelMeasurement[i] * 40.0f;
        
        float peakPositionX = (bounds.getWidth() * 0.1f) + (peakWidthScaling * ratioValNormalized[i]);
        float peakPositionY = (bounds.getHeight() * (0.95f - ampValNormalized[i] * 0.85f)); // offset affine transform
        
        std::array<float, 2> peakBottomLeft = { bounds.getX() + (i * 10.0f), bounds.getY() + bounds.getHeight() + (i * 10.0f)};
        std::array<float, 2> peakCenterLeft = { (peakBottomLeft[0] + peakPositionX - peakCurveOffset) + (i * 10.0f) , bounds.getY() + bounds.getHeight() + (i * 10.0f)};
        
        std::array<float, 2> peakCenter = { (peakBottomLeft[0] + peakPositionX) + (i * 10.0f), bounds.getY() + peakPositionY + (i * 10.0f)} ;
        
        std::array<float, 2> peakCenterRight = { (peakBottomLeft[0] + peakPositionX + peakCurveOffset) + (i * 10.0f), bounds.getY() + bounds.getHeight() + (i * 10.0f)};
        std::array<float, 2> peakBottomRight = { (bounds.getX() + bounds.getWidth()) + (i * 10.0f), bounds.getY() + bounds.getHeight() + (i * 10.0f)};
    
        //==============================================================================
        // Path
        if (i < partials){
            visualizerPath.startNewSubPath(peakBottomLeft[0], peakBottomLeft[1]);
            visualizerPath.lineTo(peakCenterLeft[0], peakCenterLeft[1]);
            visualizerPath.lineTo(peakCenter[0], peakCenter[1] + peakCenterLevel);
            visualizerPath.lineTo(peakCenterRight[0], peakCenterRight[1]);
            visualizerPath.lineTo(peakBottomRight[0], peakBottomRight[1]);
            visualizerPath = visualizerPath.createPathWithRoundedCorners(30.0f);
            
        } else {
            visualizerOffPath.startNewSubPath(peakBottomLeft[0], peakBottomLeft[1]);
            visualizerOffPath.lineTo(peakCenterLeft[0], peakCenterLeft[1]);
            visualizerOffPath.lineTo(peakCenter[0], peakCenter[1] + peakCenterLevel);
            visualizerOffPath.lineTo(peakCenterRight[0], peakCenterRight[1]);
            visualizerOffPath.lineTo(peakBottomRight[0], peakBottomRight[1]);
            visualizerOffPath = visualizerOffPath.createPathWithRoundedCorners(30.0f);
            
        }
    }
    
    juce::AffineTransform transform(0.85f, 0.0f, 0.0f, -0.15f, 0.80f, 0.0f);
    visualizerPath.applyTransform(transform);
    visualizerOffPath.applyTransform(transform);

    juce::Colour onColor(juce::Colour(233, 193, 89));
    juce::Colour offColor(juce::Colour(233, 193, 89).withAlpha(0.35f));

    g.setColour(onColor);
    g.fillPath(visualizerPath);
    g.setColour(offColor);
    g.fillPath(visualizerOffPath);

    

}

//==============================================================================

void MainGraphics::ratioValues()
{
    std::array<float, 8> ratioVal;

    float scaleScaled = (scale * 0.1f) + 1.0f;
    for (int i = 0; i < 8; i++) {
        ratioVal[i] = std::pow(i + 1, scaleScaled) + offset;
        
        if (latch){
            ratioVal[0] = 1.0f;
        }
        
        if (ratioVal[i] <= 1.0f) {
            ratioVal[i] = 1.0f;
        }
    }
    
    // normalize values
    float maxValue = ratioVal[7];
    float minValue = ratioVal[0];

    float valueRange = abs(maxValue - minValue);
    for (int i = 0; i < 8; i++){
        ratioValNormalized[i] = (ratioVal[i]/valueRange);
        
        if (ratioValNormalized[i] >= 1.0f ){
            ratioValNormalized[i] = 1.0f;
        }
    }
}

void MainGraphics::ampValues()
{
    float ampScaled = amp/100.0f + 0.15f;
    for (int i = 0; i < 8; i++){
        ampValNormalized[i] = 1.0f - abs(sin((i + 1) * ampScaled));
        if (latch){
            ampValNormalized[0] = 0.85f;
        }
    }
}

//==============================================================================

void MainGraphics::updateValues(int newPartials, float newResonance, float newScale, float newOffset, float newRound, float newAmp, bool newLatch)
{
    partials = newPartials;
    resonance = newResonance;
    scale = newScale;
    offset = newOffset;
    round = newRound;
    amp = newAmp;
    latch = newLatch;
    
    ratioValues();
    ampValues();
    
    repaint();
}

void MainGraphics::parameterValueChanged(int parameterIndex, float newValue)
{
    parametersChanged.set(true);
}
