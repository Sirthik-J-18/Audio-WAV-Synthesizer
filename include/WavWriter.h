#pragma once
#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class WavWriter {
public:
    static bool write(const std::string& filename,
                      const std::vector<double>& samples,
                      std::uint32_t sampleRate,
                      std::uint16_t channels = 1);

private:
    static std::int16_t toInt16(double sample);
    static void writeU16(std::ostream& out, std::uint16_t value);
    static void writeU32(std::ostream& out, std::uint32_t value);
};
