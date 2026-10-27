#pragma once
#include <cstddef>

class Envelope {
public:
    Envelope(double attack, double decay, double sustain, double release,
             double sampleRate);

    double amplitudeAt(std::size_t sampleIndex, std::size_t totalSamples) const;

private:
    double attack_;
    double decay_;
    double sustain_;
    double release_;
    double sampleRate_;
};
