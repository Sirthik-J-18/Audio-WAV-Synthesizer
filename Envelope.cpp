#include "Envelope.h"

Envelope::Envelope(double attack, double decay, double sustain, double release,
                   double sampleRate)
    : attack_(attack), decay_(decay), sustain_(sustain), release_(release),
      sampleRate_(sampleRate) {}

double Envelope::amplitudeAt(std::size_t sampleIndex, std::size_t totalSamples) const {
    (void)sampleIndex;
    (void)totalSamples;
    // TODO: convert times to sample counts using sampleRate_
    // TODO: attack  -> ramp 0 to 1
    // TODO: decay   -> ramp 1 to sustain_
    // TODO: sustain -> hold sustain_
    // TODO: release -> ramp sustain_ to 0 over the last release_ seconds
    return 1.0;
}
