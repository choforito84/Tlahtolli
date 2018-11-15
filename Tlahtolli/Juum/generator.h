#pragma once
#pragma comment(lib, "winmm.lib")

#ifndef __GENERATOR__
#define __GENERATOR__
#define _USE_MATH_DEFINES true

#include <functional>
#include <math.h>
#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <mmreg.h>
#include <complex>

namespace Tlahtolli {
    class Juum {
    public:
        float Pure(float seconds, unsigned short channel, void *context);
        float Triangle(float seconds, unsigned short channel, void *context);
        float Sine(float seconds, unsigned short channel, void *context);
        MMRESULT Play(float seconds, std::function<float (float, unsigned short, void*)> signal, void *context = NULL, unsigned long sps = 44100);
    private:
    };
}

#endif