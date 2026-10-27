#pragma once

class Oscillator {
public:
    Oscillator(double frequency, double sampleRate);
    virtual ~Oscillator() = default;

    virtual double nextSample() = 0;

    void setFrequency(double frequency);
    void reset();

protected:
    void advancePhase();

    double frequency_;
    double sampleRate_;
    double phase_ = 0.0;
};
