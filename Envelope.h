#pragma once
#include <cstddef>

// ADSR envelope. Times in seconds, sustain is a level between 0 and 1.
class Envelope {
public:
    Envelope(double attack, double decay, double sustain, double release,
             double sampleRate);

    // Gain (0.0 - 1.0) for the given sample of a note that is totalSamples long.
    double amplitudeAt(std::size_t sampleIndex, std::size_t totalSamples) const;

private:
    double attack_;
    double decay_;
    double sustain_;
    double release_;
    double sampleRate_;
};
