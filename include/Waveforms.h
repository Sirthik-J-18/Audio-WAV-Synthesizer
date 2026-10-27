#pragma once
#include "Oscillator.h"

class SineOscillator : public Oscillator {
public:
    using Oscillator::Oscillator;
    double nextSample() override;
};

class SquareOscillator : public Oscillator {
public:
    using Oscillator::Oscillator;
    double nextSample() override;
};

class SawtoothOscillator : public Oscillator {
public:
    using Oscillator::Oscillator;
    double nextSample() override;
};
