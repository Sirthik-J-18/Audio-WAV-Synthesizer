#include "Oscillator.h"

Oscillator::Oscillator(double frequency, double sampleRate)
    : frequency_(frequency), sampleRate_(sampleRate) {}

void Oscillator::setFrequency(double frequency) {
    frequency_ = frequency;
}

void Oscillator::reset() {
    phase_ = 0.0;
}

void Oscillator::advancePhase() {
    // TODO: phase_ += frequency_ / sampleRate_;
    // TODO: wrap phase_ back into [0, 1)
}
