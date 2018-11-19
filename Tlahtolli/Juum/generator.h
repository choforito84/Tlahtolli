#pragma comment(lib, "winmm.lib")

#ifndef __GENERATOR__
#define __GENERATOR__

#define _USE_MATH_DEFINES true

#include <functional>
#include <cmath>

#include <windows.h>
#include <mmreg.h>
#include <complex>

namespace tlahtolli {
    enum ChannelKind {
        Mono = 1,
        Stereo = 2
    };

    float sine(float seconds, const float frequency);
    float square(float seconds, const float frequency);
    float triangle(float seconds, const float frequency);
    float sawtooth(float seconds, const float frequency);

    class Juum {
    public:
        Juum(float (*signal)(float, float), int samplesPerSecond = 44100);
        ~Juum();
        void setSamplesPerSecond(int samplesPerSecond);
        void play(tlahtolli::ChannelKind, float ms, float frequency);
    private:
        unsigned int samplesPerSecond;
        std::function<float(float, const float)> signal;
        MMRESULT play(WAVEFORMATEX wave, float ms, float* buffer, const size_t bufferSize);
    };
}

#endif