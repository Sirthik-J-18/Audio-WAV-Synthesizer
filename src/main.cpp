#include "SynthSettings.h"
#include "WavWriter.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static void printUsage() {
    std::cout << "Usage: synth [--wave sine|square|saw] [--freq HZ] [--duration SEC]\n"
                 "             [--amp 0-1] [--rate HZ] [--attack S] [--decay S]\n"
                 "             [--sustain 0-1] [--release S] [--out FILE]\n";
}

static bool parseDouble(const std::string& text, double& out) {
    try {
        std::size_t used = 0;
        out = std::stod(text, &used);
        return used == text.size();
    } catch (...) {
        return false;
    }
}

static bool parseWave(const std::string& text, Waveform& out) {
    if (text == "sine") {
        out = Waveform::Sine;
    } else if (text == "square") {
        out = Waveform::Square;
    } else if (text == "saw" || text == "sawtooth") {
        out = Waveform::Sawtooth;
    } else {
        return false;
    }
    return true;
}

static bool validate(const SynthSettings& s, const std::string& outFile) {
    if (s.sampleRate < 8000 || s.sampleRate > 192000) {
        std::cerr << "Error: --rate must be between 8000 and 192000\n";
        return false;
    }
    if (s.frequency <= 0.0 || s.frequency >= s.sampleRate / 2.0) {
        std::cerr << "Error: --freq must be above 0 and below half the sample rate\n";
        return false;
    }
    if (s.duration <= 0.0 || s.duration > 600.0) {
        std::cerr << "Error: --duration must be between 0 and 600 seconds\n";
        return false;
    }
    if (s.amplitude < 0.0 || s.amplitude > 1.0) {
        std::cerr << "Error: --amp must be between 0 and 1\n";
        return false;
    }
    if (s.sustain < 0.0 || s.sustain > 1.0) {
        std::cerr << "Error: --sustain must be between 0 and 1\n";
        return false;
    }
    if (s.attack < 0.0 || s.decay < 0.0 || s.release < 0.0) {
        std::cerr << "Error: --attack, --decay and --release cannot be negative\n";
        return false;
    }
    if (outFile.empty()) {
        std::cerr << "Error: --out cannot be empty\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    SynthSettings settings;
    std::string outFile = "tone.wav";

    for (int i = 1; i < argc; ++i) {
        const std::string flag = argv[i];

        if (flag == "--help" || flag == "-h") {
            printUsage();
            return 0;
        }
        if (i + 1 >= argc) {
            std::cerr << "Error: missing value for " << flag << "\n";
            printUsage();
            return 1;
        }

        const std::string value = argv[++i];
        bool ok = true;
        double number = 0.0;

        if (flag == "--wave") {
            ok = parseWave(value, settings.waveform);
        } else if (flag == "--freq") {
            ok = parseDouble(value, settings.frequency);
        } else if (flag == "--duration") {
            ok = parseDouble(value, settings.duration);
        } else if (flag == "--amp") {
            ok = parseDouble(value, settings.amplitude);
        } else if (flag == "--rate") {
            ok = parseDouble(value, number);
            if (ok) settings.sampleRate = static_cast<int>(number);
        } else if (flag == "--attack") {
            ok = parseDouble(value, settings.attack);
        } else if (flag == "--decay") {
            ok = parseDouble(value, settings.decay);
        } else if (flag == "--sustain") {
            ok = parseDouble(value, settings.sustain);
        } else if (flag == "--release") {
            ok = parseDouble(value, settings.release);
        } else if (flag == "--out") {
            outFile = value;
        } else {
            std::cerr << "Error: unknown option " << flag << "\n";
            printUsage();
            return 1;
        }

        if (!ok) {
            std::cerr << "Error: invalid value '" << value << "' for " << flag << "\n";
            return 1;
        }
    }

    if (!validate(settings, outFile)) {
        return 1;
    }

    const std::vector<double> samples = render(settings);

    if (!WavWriter::write(outFile, samples,
                          static_cast<std::uint32_t>(settings.sampleRate))) {
        std::cerr << "Error: could not write " << outFile << "\n";
        return 1;
    }

    std::cout << "Wrote " << outFile << " (" << samples.size() << " samples)\n";
    return 0;
}
