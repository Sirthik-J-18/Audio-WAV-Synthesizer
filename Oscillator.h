#pragma once

// Abstract base class for all waveform generators.
class Oscillator {
public:
    Oscillator(double frequency, double sampleRate);
    virtual ~Oscillator() = default;   // must be virtual

    // Returns the next sample in [-1.0, 1.0] and advances the phase.
    virtual double nextSample() = 0;

    void setFrequency(double frequency);
    void reset();

protected:
    void advancePhase();               // phase_ += frequency_ / sampleRate_, wrap to [0, 1)

    double frequency_;
    double sampleRate_;
    double phase_ = 0.0;               // normalised phase in [0, 1)
};
