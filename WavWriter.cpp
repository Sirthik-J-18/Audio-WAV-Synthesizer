#include "WavWriter.h"
#include <algorithm>
#include <fstream>

std::int16_t WavWriter::toInt16(double sample) {
    // TODO: clamp to [-1, 1], multiply by 32767, return as int16_t
    (void)sample;
    return 0;
}

void WavWriter::writeU16(std::ostream& out, std::uint16_t value) {
    // TODO: write the 2 bytes of value (little-endian)
    (void)out; (void)value;
}

void WavWriter::writeU32(std::ostream& out, std::uint32_t value) {
    // TODO: write the 4 bytes of value (little-endian)
    (void)out; (void)value;
}

bool WavWriter::write(const std::string& filename,
                      const std::vector<double>& samples,
                      std::uint32_t sampleRate,
                      std::uint16_t channels) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    // TODO: compute dataSize, byteRate, blockAlign, chunkSize (36 + dataSize)
    // TODO: write the 44-byte header field by field:
    //       "RIFF", chunkSize, "WAVE", "fmt ", 16, format=1, channels,
    //       sampleRate, byteRate, blockAlign, bitsPerSample=16, "data", dataSize
    // TODO: convert each sample with toInt16 and write them after the header

    (void)samples; (void)sampleRate; (void)channels;
    return file.good();
}
