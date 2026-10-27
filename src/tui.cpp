#include "SynthSettings.h"
#include "WavWriter.h"
#include "Waveforms.h"
#include "Envelope.h"

#include <cstdint>
#include <cmath>
#include <string>
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/canvas.hpp"

namespace {

constexpr double kTwoPi = 6.283185307179586476925286766559;

// ---------------------------------------------------------------------------
// Slider value helpers — FTXUI sliders work with int, so we map to/from int.
// ---------------------------------------------------------------------------

struct SliderState {
    int waveIndex   = 0;     // 0=Sine, 1=Square, 2=Saw
    int frequency   = 440;   // 20 – 2000 Hz
    int amplitude   = 50;    // 0 – 100  (÷100 → 0.0–1.0)
    int duration    = 20;    // 1 – 100  (÷10  → 0.1–10.0 s)
    int sampleRate  = 44100; // fixed display
    int attack      = 5;     // 0 – 200  (÷100 → 0.0–2.0 s)
    int decay       = 10;    // 0 – 200
    int sustain     = 70;    // 0 – 100  (÷100 → 0.0–1.0)
    int release     = 20;    // 0 – 200
};

SynthSettings toSettings(const SliderState& s) {
    SynthSettings out;
    switch (s.waveIndex) {
        case 1:  out.waveform = Waveform::Square;   break;
        case 2:  out.waveform = Waveform::Sawtooth;  break;
        default: out.waveform = Waveform::Sine;      break;
    }
    out.frequency  = static_cast<double>(s.frequency);
    out.amplitude  = s.amplitude  / 100.0;
    out.duration   = s.duration   / 10.0;
    out.sampleRate = s.sampleRate;
    out.attack     = s.attack     / 100.0;
    out.decay      = s.decay      / 100.0;
    out.sustain    = s.sustain    / 100.0;
    out.release    = s.release    / 100.0;
    return out;
}

// ---------------------------------------------------------------------------
// Waveform preview — compute ~2 cycles of the selected waveform.
// ---------------------------------------------------------------------------

std::vector<double> previewWaveform(const SliderState& s, int numPoints) {
    std::vector<double> pts(numPoints);
    const double cycles = 2.0;
    for (int i = 0; i < numPoints; ++i) {
        double phase = (static_cast<double>(i) / numPoints) * cycles;
        phase -= std::floor(phase);   // normalise to [0,1)
        double sample = 0.0;
        switch (s.waveIndex) {
            case 0:  sample = std::sin(kTwoPi * phase); break;
            case 1:  sample = phase < 0.5 ? 1.0 : -1.0; break;
            case 2:  sample = 2.0 * phase - 1.0;         break;
            default: break;
        }
        pts[i] = sample * (s.amplitude / 100.0);
    }
    return pts;
}

// ---------------------------------------------------------------------------
// ADSR envelope preview.
// ---------------------------------------------------------------------------

std::vector<double> previewEnvelope(const SliderState& s, int numPoints) {
    const double attack  = s.attack  / 100.0;
    const double decay   = s.decay   / 100.0;
    const double sustain = s.sustain / 100.0;
    const double release = s.release / 100.0;
    const double total   = attack + decay + 1.0 + release; // 1s sustain hold

    std::vector<double> pts(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        const double t = (static_cast<double>(i) / numPoints) * total;
        double gain;
        if (t < attack) {
            gain = (attack > 0.0) ? (t / attack) : 1.0;
        } else if (t < attack + decay) {
            gain = 1.0 - (1.0 - sustain) * ((t - attack) / (decay > 0.0 ? decay : 1.0));
        } else if (t < attack + decay + 1.0) {
            gain = sustain;
        } else {
            const double rel_t = t - (attack + decay + 1.0);
            gain = sustain * (1.0 - rel_t / (release > 0.0 ? release : 1.0));
            if (gain < 0.0) gain = 0.0;
        }
        pts[i] = gain;
    }
    return pts;
}

// ---------------------------------------------------------------------------
// Render a graph on an FTXUI canvas.
// ---------------------------------------------------------------------------

ftxui::Element drawGraph(const std::vector<double>& data,
                         int width, int height,
                         double yMin, double yMax,
                         ftxui::Color color) {
    // Canvas uses 2x resolution (braille), so multiply accordingly.
    const int cw = width * 2;
    const int ch = height * 4;
    auto c = ftxui::Canvas(cw, ch);

    auto mapX = [&](int i) -> int {
        return static_cast<int>(static_cast<double>(i) / (data.size() - 1) * (cw - 1));
    };
    auto mapY = [&](double v) -> int {
        double norm = (v - yMin) / (yMax - yMin);            // 0..1
        return static_cast<int>((1.0 - norm) * (ch - 1));    // flip Y
    };

    for (std::size_t i = 1; i < data.size(); ++i) {
        c.DrawBlockLine(mapX(static_cast<int>(i - 1)), mapY(data[i - 1]),
                        mapX(static_cast<int>(i)),     mapY(data[i]),
                        color);
    }
    return ftxui::canvas(std::move(c));
}

}  // namespace

// ===========================================================================
// main
// ===========================================================================

int main() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // State -------------------------------------------------------------------
    SliderState st;
    std::string outFile = "tone.wav";
    std::string statusMsg = "Ready — adjust parameters and press Generate.";

    // Waveform toggle ---------------------------------------------------------
    std::vector<std::string> waveLabels = {"Sine", "Square", "Sawtooth"};
    auto waveToggle = Toggle(&waveLabels, &st.waveIndex);

    // Sliders -----------------------------------------------------------------
    int freqMin = 20, freqMax = 2000;
    int ampMin = 0, ampMax = 100;
    int durMin = 1, durMax = 100;
    int envMin = 0, envMax = 200;
    int susMin = 0, susMax = 100;

    auto sliderFreq    = Slider("", &st.frequency,  freqMin, freqMax, 1);
    auto sliderAmp     = Slider("", &st.amplitude,  ampMin,  ampMax,  1);
    auto sliderDur     = Slider("", &st.duration,   durMin,  durMax,  1);
    auto sliderAttack  = Slider("", &st.attack,     envMin,  envMax,  1);
    auto sliderDecay   = Slider("", &st.decay,      envMin,  envMax,  1);
    auto sliderSustain = Slider("", &st.sustain,    susMin,  susMax,  1);
    auto sliderRelease = Slider("", &st.release,    envMin,  envMax,  1);

    // Output filename input ---------------------------------------------------
    auto inputFile = Input(&outFile, "output filename");

    // Generate button ---------------------------------------------------------
    auto btnGenerate = Button("  Generate WAV  ", [&] {
        SynthSettings settings = toSettings(st);
        const std::vector<double> samples = render(settings);
        if (WavWriter::write(outFile, samples,
                             static_cast<std::uint32_t>(settings.sampleRate))) {
            statusMsg = "✓ Wrote " + outFile + " (" +
                        std::to_string(samples.size()) + " samples)";
        } else {
            statusMsg = "✗ Error: could not write " + outFile;
        }
    }, ButtonOption::Animated(Color::Green));

    // Quit button -------------------------------------------------------------
    auto btnQuit = Button("  Quit  ", screen.ExitLoopClosure(),
                          ButtonOption::Animated(Color::Red));

    // Compose the left-hand controls ------------------------------------------
    auto controls = Container::Vertical({
        waveToggle,
        sliderFreq,
        sliderAmp,
        sliderDur,
        sliderAttack,
        sliderDecay,
        sliderSustain,
        sliderRelease,
        inputFile,
        Container::Horizontal({btnGenerate, btnQuit}),
    });

    // Full renderer -----------------------------------------------------------
    auto renderer = Renderer(controls, [&] {
        // Build value labels
        auto labeledSlider = [](std::string label, Element slider, std::string value) {
            return hbox({
                text(label) | size(WIDTH, EQUAL, 12),
                slider | flex,
                text(" " + value) | size(WIDTH, EQUAL, 10),
            });
        };

        // Waveform preview graph
        auto waveData = previewWaveform(st, 120);
        auto waveGraph = drawGraph(waveData, 50, 8, -1.0, 1.0, Color::Cyan);

        // ADSR envelope graph
        auto envData = previewEnvelope(st, 120);
        auto envGraph = drawGraph(envData, 50, 8, 0.0, 1.0, Color::Yellow);

        // Left panel: controls
        auto leftPanel = vbox({
            text("  Waveform") | bold,
            waveToggle->Render() | color(Color::Cyan),
            separator(),
            labeledSlider("  Frequency",
                          sliderFreq->Render(),
                          std::to_string(st.frequency) + " Hz"),
            labeledSlider("  Amplitude",
                          sliderAmp->Render(),
                          std::to_string(st.amplitude) + "%"),
            labeledSlider("  Duration",
                          sliderDur->Render(),
                          std::to_string(st.duration) + "ds"),
            separator(),
            text("  ADSR Envelope") | bold,
            labeledSlider("  Attack",
                          sliderAttack->Render(),
                          std::to_string(st.attack) + "cs"),
            labeledSlider("  Decay",
                          sliderDecay->Render(),
                          std::to_string(st.decay) + "cs"),
            labeledSlider("  Sustain",
                          sliderSustain->Render(),
                          std::to_string(st.sustain) + "%"),
            labeledSlider("  Release",
                          sliderRelease->Render(),
                          std::to_string(st.release) + "cs"),
            separator(),
            hbox({
                text("  Output: ") | bold,
                inputFile->Render() | flex | border,
            }),
            filler(),
            hbox({
                btnGenerate->Render(),
                text("  "),
                btnQuit->Render(),
            }) | center,
        });

        // Right panel: graphs
        auto rightPanel = vbox({
            text("  Waveform Preview") | bold | color(Color::Cyan),
            waveGraph | flex,
            separator(),
            text("  ADSR Envelope") | bold | color(Color::Yellow),
            envGraph | flex,
            separator(),
            text("  " + statusMsg) | (statusMsg[0] == '\xe2'
                                        ? color(Color::Green)
                                        : color(Color::White)),
        });

        return vbox({
            text("  ♪ Audio WAV Synthesizer ♪") | bold | center | color(Color::Magenta),
            separator(),
            hbox({
                leftPanel | size(WIDTH, EQUAL, 50) | border,
                rightPanel | flex | border,
            }) | flex,
        }) | border;
    });

    screen.Loop(renderer);
    return 0;
}
