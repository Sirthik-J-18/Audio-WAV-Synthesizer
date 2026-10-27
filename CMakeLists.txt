
cmake_minimum_required(VERSION 3.14)
project(synth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- Core library (shared between CLI and TUI) ----------
add_library(synth_core STATIC
    src/Oscillator.cpp
    src/Waveforms.cpp
    src/Envelope.cpp
    src/WavWriter.cpp
    src/Synth.cpp
)
target_include_directories(synth_core PUBLIC include)
if(NOT MSVC)
    target_compile_options(synth_core PRIVATE -Wall -Wextra)
endif()

# ---------- CLI executable (unchanged) ----------
add_executable(synth src/main.cpp)
target_link_libraries(synth PRIVATE synth_core)
if(NOT MSVC)
    target_compile_options(synth PRIVATE -Wall -Wextra)
endif()

# ---------- FTXUI (fetched automatically) ----------
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# ---------- TUI executable ----------
add_executable(synth-tui src/tui.cpp)
target_link_libraries(synth-tui
    PRIVATE
        synth_core
        ftxui::screen
        ftxui::dom
        ftxui::component
)
if(NOT MSVC)
    target_compile_options(synth-tui PRIVATE -Wall -Wextra)
endif()
