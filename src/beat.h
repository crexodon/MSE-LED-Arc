#include "globals.h"

// Arduino Beat Detector By Damian Peckett 2015
// License: Public Domain.

// Our Global Sample Rate, 5000hz
#define SAMPLEPERIODUS 200

// 20 - 200hz Single Pole Bandpass IIR Filter
float bassFilter(float sample) {
    static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
    xv[0] = xv[1];
    xv[1] = xv[2];
    xv[2] = (sample) / 3.f; // change here to values close to 2, to adapt for stronger or weeker sources of line level audio

    yv[0] = yv[1];
    yv[1] = yv[2];
    yv[2] = (xv[2] - xv[0]) + (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
    return yv[2];
}

// 10hz Single Pole Lowpass IIR Filter
float envelopeFilter(float sample) { // 10hz low pass
    static float xv[2] = {0, 0}, yv[2] = {0, 0};
    xv[0] = xv[1];
    xv[1] = sample / 50.f;
    yv[0] = yv[1];
    yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
    return yv[1];
}

// 1.7 - 3.0hz Single Pole Bandpass IIR Filter
float beatFilter(float sample) {
    static float xv[3] = {0, 0, 0}, yv[3] = {0, 0, 0};
    xv[0] = xv[1];
    xv[1] = xv[2];
    xv[2] = sample / 2.7f;
    yv[0] = yv[1];
    yv[1] = yv[2];
    yv[2] = (xv[2] - xv[0]) + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
    return yv[2];
}

float sample, value, envelope, beat, thresh;
uint8_t sampleIdx;
uint32_t lastBeatLoop = 0;

void beatLoop() {
    if (micros() - lastBeatLoop >= SAMPLEPERIODUS) {
        lastBeatLoop = micros();
        ++sampleIdx;

        // Read ADC and center so +-2048
        sample = (float)analogRead(MIC_PIN) - 503.f;

        // Filter only bass component
        value = bassFilter(sample);

        // Take signal amplitude and filter
        if (value < 0)
            value = -value;
        envelope = envelopeFilter(value);

        // Every 200 samples (25hz) filter the envelope
        if (sampleIdx == 200) {
            // Filter out repeating bass sounds 100 - 180bpm
            beat = beatFilter(envelope);

            // Threshold it based on potentiometer on AN1
            // thresh = 0.02f * (float)analogRead(1);
            thresh = 200;

            // If we are above threshold, light up LED
            if (beat > thresh)
                digitalWrite(BUILTIN_LED, HIGH);
            else
                digitalWrite(BUILTIN_LED, LOW);

            // Reset sample counter
            sampleIdx = 0;
            Serial.printf("\n%f,%f,%f,%d,%d", beat, thresh, value, 1000, -1000);
        }

        // Serial.printf("\n%f,%f,%f,%f,%d,%d", sample - 2048, value, envelope, beat, 1000, -1000);

        // Consume excess clock cycles, to keep at 5000 hz
        // for (unsigned long up = time + SAMPLEPERIODUS; time > 20 && time < up; time = micros())
        //     ;
    }
}