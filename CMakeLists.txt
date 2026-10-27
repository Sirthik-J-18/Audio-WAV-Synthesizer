
cmake_minimum_required(VERSION 3.10)
project(synth LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(synth
    src/main.cpp
    src/Oscillator.cpp
    src/Waveforms.cpp
    src/Envelope.cpp
    src/WavWriter.cpp
    src/Synth.cpp
)
target_include_directories(synth PRIVATE include)
if(NOT MSVC)
    target_compile_options(synth PRIVATE -Wall -Wextra)
endif()
