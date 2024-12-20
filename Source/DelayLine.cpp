#include <JuceHeader.h>
#include "DelayLine.h"

void DelayLine::setMaximumDelayInSamples(int maxDelayInSamples)
{
    jassert(maxDelayInSamples > 0);
    
    int paddedLength = maxDelayInSamples + 2;
    if (bufferLength < paddedLength) {
        bufferLength = paddedLength;
        
        buffer.reset(new float[size_t(bufferLength)]);
    }
}

void DelayLine::reset() noexcept
{
    writeIndex = bufferLength - 1;
    
    for (size_t i = 0; i < size_t(bufferLength); ++i){
        buffer[i] = 0.0f;
    }
}

void DelayLine::write(float input) noexcept
{
    jassert(bufferLength > 0);
    writeIndex += 1;
    if (writeIndex >= bufferLength){
        writeIndex = 0;
    }
    
    buffer[size_t(writeIndex)] = input;
}

float DelayLine::read(float delayInSamples) const noexcept
{
    jassert(delayInSamples >= 0.0f);
    jassert(delayInSamples <= bufferLength - 1.0f);
    
    int readIndex = int(std::round(writeIndex - delayInSamples));
    
    if (readIndex < 0) {
        readIndex += bufferLength;
    }
    
    return buffer[size_t(readIndex)];
}
