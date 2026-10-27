#pragma once
#include <vector>

enum class Waveform { Sine, Square, Sawtooth };

struct SynthSettings {
    Waveform waveform   = Waveform::Sine;
    double   frequency  = 440.0;
    double   duration   = 2.0;
    double   amplitude  = 0.5;
    int      sampleRate = 44100;
    double   attack     = 0.05;
    double   decay      = 0.10;
    double   sustain    = 0.70;
    double   release    = 0.20;
};

std::vector<double> render(const SynthSettings& settings);
