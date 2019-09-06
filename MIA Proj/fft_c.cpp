//fft_c.cpp
//fft function
//copyright @ MIA project
//version 0.0

#include "fft_c.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
extern voicesensor voicesensor1;

const uint16_t samples = 128;
const double samplingFrequency = 2048;

unsigned int sampling_period_us;
unsigned long microseconds;

double vReal[samples];
double vImag[samples];

double fuckingCompute()
{
    unsigned int sampling_period_us = round(1000000 * (1.0 / samplingFrequency));
    unsigned long microseconds = micros();
    uint8_t volumeTest = 0;

    //判断声音强度;
    for (uint8_t i = 0; i < 10; i++)
    {
        if (voicesensor1.hasvoice())
        {
            volumeTest++;
            delayMicroseconds(100);
        }
        else
        {
            delayMicroseconds(250);
        }
        if(volumeTest>=3)
        {
            break;
        }
    }
    if (volumeTest >= 3)
    {
        for (int i = 0; i < samples; i++)
        {
            vReal[i] = analogRead(CHANNEL);
            vImag[i] = 0;
            while (micros() - microseconds < sampling_period_us)
            {
                //empty loop
            }
            microseconds += sampling_period_us;
        }
        /* Print the results of the sampling according to time */
        FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD); /* Weigh data */
        FFT.Compute(vReal, vImag, samples, FFT_FORWARD);                 /* Compute FFT */
        FFT.ComplexToMagnitude(vReal, vImag, samples);                   /* Compute magnitudes */
        double fftResult = FFT.MajorPeak(vReal, samples, samplingFrequency);
        return fftResult; //Print out what frequency is the most dominant.
    }
    else
    {
        return 0.0;
    }
}
