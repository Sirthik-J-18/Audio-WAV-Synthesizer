#include "SynthSettings.h"
#include "Envelope.h"
#include "Waveforms.h"
#include <cstddef>
#include <memory>

static std::unique_ptr<Oscillator> makeOscillator(const SynthSettings& s) {
    switch (s.waveform) {
        case Waveform::Square:
            return std::make_unique<SquareOscillator>(s.frequency, s.sampleRate);
        case Waveform::Sawtooth:
            return std::make_unique<SawtoothOscillator>(s.frequency, s.sampleRate);
        case Waveform::Sine:
        default:
            return std::make_unique<SineOscillator>(s.frequency, s.sampleRate);
    }
}

std::vector<double> render(const SynthSettings& s) {
    const std::size_t total = static_cast<std::size_t>(s.duration * s.sampleRate);

    std::unique_ptr<Oscillator> osc = makeOscillator(s);
    Envelope env(s.attack, s.decay, s.sustain, s.release, s.sampleRate);

    std::vector<double> samples;
    samples.reserve(total);

    for (std::size_t i = 0; i < total; ++i) {
        samples.push_back(osc->nextSample() * env.amplitudeAt(i, total) * s.amplitude);
    }
    return samples;
}
