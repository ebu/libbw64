#include <catch2/catch.hpp>
#include <sstream>
#include "bw64/bw64.hpp"

using namespace bw64;

TEST_CASE("rect_16bit") {
  Bw64Reader bw64File("rect_16bit.wav");
  REQUIRE(bw64File.bitDepth() == 16);
  REQUIRE(bw64File.sampleRate() == 44100);
  REQUIRE(bw64File.channels() == 2);
  REQUIRE(bw64File.numberOfFrames() == 22050);
}

TEST_CASE("rect_24bit") {
  Bw64Reader bw64File("rect_24bit.wav");
  REQUIRE(bw64File.bitDepth() == 24);
  REQUIRE(bw64File.sampleRate() == 44100);
  REQUIRE(bw64File.channels() == 2);
  REQUIRE(bw64File.numberOfFrames() == 22050);
}

TEST_CASE("rect_32bit") {
  Bw64Reader bw64File("rect_32bit.wav");
  REQUIRE(bw64File.bitDepth() == 32);
  REQUIRE(bw64File.sampleRate() == 44100);
  REQUIRE(bw64File.channels() == 2);
  REQUIRE(bw64File.numberOfFrames() == 22050);
}

TEST_CASE("rect_24bit_rf64") {
  Bw64Reader bw64File("rect_24bit_rf64.wav");
  REQUIRE(bw64File.bitDepth() == 24);
  REQUIRE(bw64File.sampleRate() == 44100);
  REQUIRE(bw64File.channels() == 2);
  REQUIRE(bw64File.numberOfFrames() == 22050);
}

TEST_CASE("rect_24bit_noriff") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_noriff.wav"), std::runtime_error);
}

TEST_CASE("rect_24bit_nowave") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_nowave.wav"), std::runtime_error);
}

TEST_CASE("rect_24bit_wrong_fmt_size") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_wrong_fmt_size.wav"),
                    std::runtime_error);
}

TEST_CASE("noise_24bit_uneven_data_chunk_size") {
  Bw64Reader bw64File("noise_24bit_uneven_data_chunk_size.wav");
  REQUIRE(bw64File.bitDepth() == 24);
  REQUIRE(bw64File.sampleRate() == 44100);
  REQUIRE(bw64File.channels() == 1);
  REQUIRE(bw64File.numberOfFrames() == 13);
  REQUIRE(bw64File.chnaChunk() != nullptr);  // XXX
}

TEST_CASE("write_16bit") {
  auto bw64File = writeFile("zeros_16bit.wav", 2u, 48000u, 16u);

  REQUIRE(bw64File->channels() == 2);
  REQUIRE(bw64File->sampleRate() == 48000);
  REQUIRE(bw64File->bitDepth() == 16);

  int frames = 4800;
  std::vector<float> data(frames * bw64File->channels(), 0.0);
  bw64File->write(&data[0], frames);
  REQUIRE(bw64File->framesWritten() == frames);
}

TEST_CASE("write_24bit") {
  auto bw64File = writeFile("zeros_24bit.wav", 2u, 48000u, 24u);

  REQUIRE(bw64File->channels() == 2);
  REQUIRE(bw64File->sampleRate() == 48000);
  REQUIRE(bw64File->bitDepth() == 24);

  int frames = 4800;
  std::vector<float> data(frames * bw64File->channels(), 0.0);
  bw64File->write(&data[0], frames);
  REQUIRE(bw64File->framesWritten() == frames);
}

TEST_CASE("write_32bit") {
  auto bw64File = writeFile("zeros_32bit.wav", 2u, 48000u, 32u);

  REQUIRE(bw64File->channels() == 2);
  REQUIRE(bw64File->sampleRate() == 48000);
  REQUIRE(bw64File->bitDepth() == 32);

  int frames = 4800;
  std::vector<float> data(frames * bw64File->channels(), 0.0);
  bw64File->write(&data[0], frames);
  REQUIRE(bw64File->framesWritten() == frames);
}

void writeClipped(const std::string& filename, uint16_t bitDepth,
                  uint64_t frames, uint16_t channels = 1u,
                  uint16_t sampleRate = 48000u) {
  auto bw64File = writeFile(filename, channels, sampleRate, bitDepth);
  std::vector<float> data(frames * channels, 2.f);
  bw64File->write(&data[0], frames);
}

TEST_CASE("write_read_16bit_clipped") {
  int frames = 4800;
  writeClipped("write_read_16bit_clipped.wav", 16, frames);
  auto bw64File = readFile("write_read_16bit_clipped.wav");
  std::vector<float> data(frames * bw64File->channels(), 0.f);
  auto readFrames = bw64File->read(&data[0], frames);
  REQUIRE(readFrames == frames);
  REQUIRE(data.at(0) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(100) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(200) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(400) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(800) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(1600) == Approx(1.f).epsilon(1e-2));
  REQUIRE(data.at(3200) == Approx(1.f).epsilon(1e-2));
}

TEST_CASE("write_read_24bit_clipped") {
  int frames = 4800;
  writeClipped("write_read_24bit_clipped.wav", 24, frames);
  auto bw64File = readFile("write_read_24bit_clipped.wav");
  std::vector<float> data(frames * bw64File->channels(), 0.f);
  auto readFrames = bw64File->read(&data[0], frames);
  REQUIRE(readFrames == frames);
  REQUIRE(data.at(0) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(100) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(200) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(400) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(800) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(1600) == Approx(1.f).epsilon(1e-3));
  REQUIRE(data.at(3200) == Approx(1.f).epsilon(1e-3));
}

TEST_CASE("write_read_32bit_clipped") {
  int frames = 4800;
  writeClipped("write_read_32bit_clipped.wav", 32, frames);
  auto bw64File = readFile("write_read_32bit_clipped.wav");
  std::vector<float> data(frames * bw64File->channels(), 0.f);
  auto readFrames = bw64File->read(&data[0], frames);
  REQUIRE(readFrames == frames);
  REQUIRE(data.at(0) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(100) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(200) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(400) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(800) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(1600) == Approx(1.f).epsilon(1e-6));
  REQUIRE(data.at(3200) == Approx(1.f).epsilon(1e-6));
}

TEST_CASE("can_read_all_frames") {
  int frames = 13;
  auto bw64File = readFile("noise_24bit_uneven_data_chunk_size.wav");
  REQUIRE(bw64File->numberOfFrames() == 13);
  std::vector<float> data(frames * bw64File->channels(), 0.f);
  int readSampleCount = bw64File->read(&data[0], frames);
  REQUIRE(readSampleCount == 13);
}
