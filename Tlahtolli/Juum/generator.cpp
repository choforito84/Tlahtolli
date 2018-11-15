#include "generator.h"

float Tlahtolli::Juum::Pure(float seconds, unsigned short channel, void * context)
{
    const float frequency = *(const float*)context;
    const float angle = (float)(frequency * 2 * M_PI * seconds);
    return (float)sin(angle + 1 * M_PI / 2);
}

float Tlahtolli::Juum::Triangle(float seconds, unsigned short channel, void * context)
{
    const float frequency = *(const float*)context;
    const float angle = (float)(frequency * 2 * M_PI * seconds);
    return (float)asin(sin(angle + 0 * M_PI / 2)) * 2 / M_PI;
}

float Tlahtolli::Juum::Sine(float seconds, unsigned short channel, void * context)
{
    return 0.0f;
}

MMRESULT Tlahtolli::Juum::Play(float seconds, std::function<float(float, unsigned short, void*)> signal, void * context, unsigned long sps)
{
    return MMRESULT();
}
