#include "WavWriter.h"
#include <algorithm>
#include <cmath>
#include <fstream>

std::int16_t WavWriter::toInt16(double sample) {
    sample = std::clamp(sample, -1.0, 1.0);
    return static_cast<std::int16_t>(std::lround(sample * 32767.0));
}

void WavWriter::writeU16(std::ostream& out, std::uint16_t value) {
    const char bytes[2] = {
        static_cast<char>(value & 0xFF),
        static_cast<char>((value >> 8) & 0xFF)
    };
    out.write(bytes, 2);
}

void WavWriter::writeU32(std::ostream& out, std::uint32_t value) {
    const char bytes[4] = {
        static_cast<char>(value & 0xFF),
        static_cast<char>((value >> 8) & 0xFF),
        static_cast<char>((value >> 16) & 0xFF),
        static_cast<char>((value >> 24) & 0xFF)
    };
    out.write(bytes, 4);
}

bool WavWriter::write(const std::string& filename,
                      const std::vector<double>& samples,
                      std::uint32_t sampleRate,
                      std::uint16_t channels) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    const std::uint16_t bitsPerSample = 16;
    const std::uint16_t blockAlign = static_cast<std::uint16_t>(channels * bitsPerSample / 8);
    const std::uint32_t byteRate = sampleRate * blockAlign;
    const std::uint32_t dataSize = static_cast<std::uint32_t>(samples.size() * 2);
    const std::uint32_t chunkSize = 36 + dataSize;

    file.write("RIFF", 4);
    writeU32(file, chunkSize);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    writeU32(file, 16);
    writeU16(file, 1);
    writeU16(file, channels);
    writeU32(file, sampleRate);
    writeU32(file, byteRate);
    writeU16(file, blockAlign);
    writeU16(file, bitsPerSample);
    file.write("data", 4);
    writeU32(file, dataSize);

    std::vector<char> buffer(samples.size() * 2);
    for (std::size_t i = 0; i < samples.size(); ++i) {
        const std::uint16_t v = static_cast<std::uint16_t>(toInt16(samples[i]));
        buffer[2 * i]     = static_cast<char>(v & 0xFF);
        buffer[2 * i + 1] = static_cast<char>((v >> 8) & 0xFF);
    }
    file.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));

    file.close();
    return !file.fail();
}
