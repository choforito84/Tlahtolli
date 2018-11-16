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
        Mono,
        Stereo
    };

    class Juum {
    public:
        Juum(int samplesPerSecond = 44100);
        ~Juum();

        void play(float ms, std::function<float(float, void*)> signal, tlahtolli::ChannelKind, void* context = NULL);

        float sine(float seconds, void *context);
        float square(float seconds, void *context);
        float triangle(float seconds, void *context);
        float sawtooth(float seconds, void *context);
    private:
        unsigned int samplesPerSecond;

        MMRESULT play(WAVEFORMATEX wave, float ms, float* buffer, const size_t bufferSize);
    };
}

#endif