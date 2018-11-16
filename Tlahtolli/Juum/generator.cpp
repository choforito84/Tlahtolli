#include "generator.h"

tlahtolli::Juum::Juum(int samplesPerSecond)
{
    this->samplesPerSecond = samplesPerSecond;
}

tlahtolli::Juum::~Juum()
{
}

float tlahtolli::Juum::sine(float seconds, void * context)
{
    return 0.0f;
}

float tlahtolli::Juum::square(float seconds, void * context)
{
    return 0.0f;
}

float tlahtolli::Juum::triangle(float seconds, void * context)
{
    const float frequency = *(const float*)context;
    const float angle = (float)(frequency * 2 * M_PI * seconds);
    return (float)asin(sin(angle)) * 2 / M_PI;
}

float tlahtolli::Juum::sawtooth(float seconds, void * context)
{
    return 0.0f;
}

void tlahtolli::Juum::play(float ms, std::function<float (float, void*)> signal, ChannelKind channel, void *context)
{
    float secs = (float)(ms / 1000.0f);
    const size_t bufferSize =
        (size_t)(secs * channel * this->samplesPerSecond);
    float *buffer = (float *)calloc(bufferSize, sizeof(*buffer));

    for (size_t i = 0; i < bufferSize; i += channel)
        for (unsigned short j = 0; j < channel; j++)
            buffer[i + j] = signal((i + j) * secs / bufferSize, context);

    WORD bitsPerSample = CHAR_BIT * sizeof(buffer[0]);
    WAVEFORMATEX wave = {
        WAVE_FORMAT_IEEE_FLOAT,
        (int)channel,
        this->samplesPerSecond,
        this->samplesPerSecond * (int)channel * bitsPerSample / CHAR_BIT,
        (int)channel * bitsPerSample / CHAR_BIT,
        bitsPerSample,
        0
    };

    play(wave, ms, buffer, bufferSize);
}

MMRESULT tlahtolli::Juum::play(WAVEFORMATEX wave, float ms, float* buffer, const size_t bufferSize)
{
    MMRESULT mmresult = MMSYSERR_ERROR;
    __try {
        HWAVEOUT hWaveOut = NULL;
        mmresult = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wave, NULL, NULL, CALLBACK_NULL);
        if (mmresult == MMSYSERR_NOERROR) {
            __try {
                timeBeginPeriod(UINT(1));
                __try {
                    WAVEHDR waveHeader = { 0 };
                    waveHeader.dwBufferLength = (ULONG)(bufferSize * sizeof(buffer[0]));
                    waveHeader.lpData = (LPSTR)&buffer[0];
                    mmresult = waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(waveHeader));
                    if (mmresult == MMSYSERR_NOERROR) {
                        __try {
                            ULONG start = GetTickCount();
                            mmresult = waveOutWrite(hWaveOut, &waveHeader, sizeof(waveHeader));
                            Sleep((ULONG)(ms - (GetTickCount() - start)));
                        }
                        __finally {
                            waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(waveHeader));
                        }
                    }
                }
                __finally {
                    timeEndPeriod(UINT(1));
                }
            }
            __finally {
                waveOutClose(hWaveOut);
            }
        }
    }
    __finally {
        free(buffer);
    }

    return mmresult;
}
