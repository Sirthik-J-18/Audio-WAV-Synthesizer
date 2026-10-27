#include "Waveforms.h"
#include <cmath>

namespace {
constexpr double kTwoPi = 6.283185307179586476925286766559;
}

double SineOscillator::nextSample() {
    const double sample = std::sin(kTwoPi * phase_);
    advancePhase();
    return sample;
}

double SquareOscillator::nextSample() {
    const double sample = phase_ < 0.5 ? 1.0 : -1.0;
    advancePhase();
    return sample;
}

double SawtoothOscillator::nextSample() {
    const double sample = 2.0 * phase_ - 1.0;
    advancePhase();
    return sample;
}
