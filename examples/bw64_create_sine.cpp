#include <cmath>
#include <iostream>
#include <bw64/bw64.hpp>

const float DURATION = 10.f;  // seconds
const float FREQUENCY = 440.f;  // Hz
const float GAIN = 0.5f;  // Hz
const uint32_t BLOCK_SIZE = 512;
const float PI = 3.14159265358979323846f;

using namespace bw64;

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [BW64_OUTPUT_FILE]" << std::endl;
    exit(1);
  }

  auto bw64File = writeFile(argv[1]);

  uint32_t frameDuration =
      static_cast<uint32_t>(DURATION * bw64File->sampleRate());
  float sineState = 0.f;
  float normF =
      static_cast<float>(2.f * PI * FREQUENCY / bw64File->sampleRate());
  while (bw64File->framesWritten() < frameDuration) {
    std::vector<float> buffer;
    for (uint32_t i = 0; i < BLOCK_SIZE; ++i) {
      sineState = static_cast<float>(std::fmod(sineState + normF, 2.f * PI));
      auto value = static_cast<float>(std::sin(sineState) * GAIN);
      buffer.push_back(value);
    }
    bw64File->write(&buffer.front(), BLOCK_SIZE);
  }
  return 0;
}
