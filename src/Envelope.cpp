#include "Envelope.h"
#include <algorithm>

Envelope::Envelope(double attack, double decay, double sustain, double release,
                   double sampleRate)
    : attack_(attack), decay_(decay), sustain_(sustain), release_(release),
      sampleRate_(sampleRate) {}

double Envelope::amplitudeAt(std::size_t sampleIndex, std::size_t totalSamples) const {
    const double t = static_cast<double>(sampleIndex) / sampleRate_;
    const double length = static_cast<double>(totalSamples) / sampleRate_;

    double gain;
    if (attack_ > 0.0 && t < attack_) {
        gain = t / attack_;
    } else if (decay_ > 0.0 && t < attack_ + decay_) {
        gain = 1.0 - (1.0 - sustain_) * ((t - attack_) / decay_);
    } else {
        gain = sustain_;
    }

    if (release_ > 0.0) {
        gain *= std::clamp((length - t) / release_, 0.0, 1.0);
    }
    return gain;
}
