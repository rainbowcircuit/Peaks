#pragma once
#include <memory>

class DelayLine {
public:
    void setMaximumDelayInSamples(int maxDelayInSamples);
    void reset() noexcept;
    void write(float input) noexcept;
    float read(float delayInSamples) const noexcept;
    int getBufferLength()
    {
        return bufferLength;
    };
    
private:
    std::unique_ptr<float[]> buffer;
    int bufferLength = 0;
    int writeIndex = 0;
};
