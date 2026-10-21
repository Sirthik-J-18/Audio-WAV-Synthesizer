#include "Waveforms.h"
#include <cmath>

double SineOscillator::nextSample() {
    // TODO: sample = sin(2 * pi * phase_); then advancePhase()
    advancePhase();
    return 0.0;
}

double SquareOscillator::nextSample() {
    // TODO: sample = phase_ < 0.5 ? 1.0 : -1.0; then advancePhase()
    advancePhase();
    return 0.0;
}

double SawtoothOscillator::nextSample() {
    // TODO: sample = 2.0 * phase_ - 1.0; then advancePhase()
    advancePhase();
    return 0.0;
}
