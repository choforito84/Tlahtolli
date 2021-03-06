#include "generator.h"

tlahtolli::Juum::Juum(float(*signal)(float, float), int samplesPerSecond)
{
    this->signal = *signal;
    this->samplesPerSecond = samplesPerSecond;
}

tlahtolli::Juum::~Juum()
{
}

float tlahtolli::sine(float seconds, const float frequency)
{
    const float angle = (float)(frequency * 2 * M_PI * seconds);
    return (float)sin(angle);
}

float tlahtolli::square(float seconds, const float frequency)
{
    const float angle = (float)(frequency * 2 * M_PI *seconds);
    return (float)std::copysign(1.0, sin(angle));
}

float tlahtolli::triangle(float seconds, const float frequency)
{
    const float angle = (float)(frequency * 2 * M_PI * seconds);
    return (float)asin(sin(angle)) * 2 / M_PI;
}

float tlahtolli::sawtooth(float seconds, const float frequency)
{
    //Angle here doesn't require angular frecuency
    const float angle = (float)(frequency * seconds);
    return (float)angle - floor(angle);
}

void tlahtolli::Juum::setSamplesPerSecond(int samplesPerSecond)
{
    this->samplesPerSecond = samplesPerSecond;
}
#ifdef __linux__ 

   void tlahtolli::Juum::play(ChannelKind channel, float ms, float frequency)
    {
        snd_pcm_t *handle;
        snd_pcm_hw_params_t *params;
        snd_pcm_uframes_t frames;
        int dir;
        int bufferSize = 44.1 * ms * channel;
        float buffer[bufferSize];
        for (int k=0; k<bufferSize/channel; k++) {
            for (int c=0; c<channel; c++){
                buffer[channel*k+c] = this->signal(k/(44100.0),   frequency);
            }
        }
        int rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if (rc < 0) {
            fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
            exit(1);
        }
        snd_pcm_hw_params_alloca(&params);
        snd_pcm_hw_params_any(handle, params);
        snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(handle, params,SND_PCM_FORMAT_FLOAT);
        snd_pcm_hw_params_set_channels(handle, params, channel);
        unsigned val = 44100;
        snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir );
        frames = 32;
        snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir );
        rc = snd_pcm_hw_params(handle, params);
        if (rc < 0) {
            fprintf(stderr, "unable to set hw parameters: %s\n",snd_strerror(rc));
            exit(1);
        }
        rc = snd_pcm_writei(handle,buffer, bufferSize);
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
        free(buffer);
    }

#elif _WIN32

void tlahtolli::Juum::play(ChannelKind channel, float ms, float frequency)
{
    float secs = (float)(ms / 1000.0f);
    const size_t bufferSize =
        (size_t)(secs * channel * this->samplesPerSecond);
    float *buffer = (float*)calloc(bufferSize, sizeof(*buffer));

    for (size_t i = 0; i < bufferSize; i += channel)
        for (unsigned short j = 0; j < channel; j++)
            buffer[i + j] = this->signal((i + j) * secs / bufferSize, frequency);

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

#endif