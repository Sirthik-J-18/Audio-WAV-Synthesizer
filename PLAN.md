Audio Synthesizer (WAV Generator): Project Plan

1.Decisions So Far:

    Library : Standard Libraries for the core logic
    Primary Interface : CLI based
    Optional-Library : FTXUI (Terminal User Interface) can be used if time permits
    Build-Order: Core Logic first then the CLI Interface or TUI

2.Libraries:
    <fstream> : binary file output
    <cstdint> : fixed width integer types for the header
    <cmath> 
    <vector> : sample buffer
    <memory>
    <string>

3.Architecture
    Oscilattor (abstract)
        SineOscillator
        SquareOscillator
        SawtoothOscilattor
    
    Envelope(ADSR)
    WavWriter
    SynthSettings + render()
    main.cpp/cli/tui
