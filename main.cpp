#include "SynthSettings.h"
#include "WavWriter.h"
#include <iostream>
#include <string>

static void printUsage() {
    std::cout << "Usage: synth [--wave sine|square|saw] [--freq HZ] [--duration SEC]\n"
                 "             [--amp 0-1] [--rate HZ] [--attack S] [--decay S]\n"
                 "             [--sustain 0-1] [--release S] [--out FILE]\n";
}

int main(int argc, char* argv[]) {
    SynthSettings settings;
    std::string outFile = "tone.wav";

    // TODO: parse argv into settings and outFile (handle --help and invalid input)
    (void)argc; (void)argv;
    (void)printUsage;

    // TODO: validate ranges (frequency, duration, amplitude, sustain, ...)

    std::vector<double> samples = render(settings);

    if (!WavWriter::write(outFile, samples,
                          static_cast<std::uint32_t>(settings.sampleRate))) {
        std::cerr << "Error: could not write " << outFile << "\n";
        return 1;
    }

    std::cout << "Wrote " << outFile << " (" << samples.size() << " samples)\n";
    return 0;
}
