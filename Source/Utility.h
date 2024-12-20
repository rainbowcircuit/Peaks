#pragma once

class EnvelopeSlew
{
public:
    float processSample(float input) noexcept
    {
        z1 += (input - z1) / 1.0f;
        return z1;
    }

private:
    float z1 = 0.0f;
};


class DCBlock
{
public:
    float processSample(float input) noexcept
    {
        float y = input - z1 + z2 * 0.9997;
        // reset unit delays
        z1 = input;
        z2 = y;
        return y;
    }
private:
    float z1 = 0.0f;
    float z2 = 0.0f;
};



