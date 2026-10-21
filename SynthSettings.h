#pragma once
#include <vector>

enum class Waveform { Sine, Square, Sawtooth };

// The contract between any front end (CLI/TUI/GUI) and the synth core.
struct SynthSettings {
    Waveform waveform   = Waveform::Sine;
    double   frequency  = 440.0;   // Hz
    double   duration   = 2.0;     // seconds
    double   amplitude  = 0.5;     // 0.0 - 1.0
    int      sampleRate = 44100;   // Hz

    // ADSR (times in seconds, sustain is a 0.0 - 1.0 level)
    double attack  = 0.05;
    double decay   = 0.10;
    double sustain = 0.70;
    double release = 0.20;
};

// Renders the settings into samples in the range [-1.0, 1.0].
std::vector<double> render(const SynthSettings& settings);
