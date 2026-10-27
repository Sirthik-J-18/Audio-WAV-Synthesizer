#include "Oscillator.h"
#include <cmath>

Oscillator::Oscillator(double frequency, double sampleRate)
    : frequency_(frequency), sampleRate_(sampleRate) {}

void Oscillator::setFrequency(double frequency) {
    frequency_ = frequency;
}

void Oscillator::reset() {
    phase_ = 0.0;
}

void Oscillator::advancePhase() {
    phase_ += frequency_ / sampleRate_;
    phase_ -= std::floor(phase_);
}
