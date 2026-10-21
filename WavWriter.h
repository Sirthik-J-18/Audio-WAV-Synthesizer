#pragma once
#include <cstdint>
#include <string>
#include <vector>

// Writes 16-bit PCM WAV files using binary file streams.
class WavWriter {
public:
    // samples are doubles in [-1.0, 1.0]; returns false if the file can't be written.
    static bool write(const std::string& filename,
                      const std::vector<double>& samples,
                      std::uint32_t sampleRate,
                      std::uint16_t channels = 1);

private:
    static std::int16_t toInt16(double sample);   // clamp, scale by 32767, convert

    // Little-endian field writers (use these field by field, not a whole struct).
    static void writeU16(std::ostream& out, std::uint16_t value);
    static void writeU32(std::ostream& out, std::uint32_t value);
};
