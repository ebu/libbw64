#include <catch2/catch.hpp>
#include <sstream>
#include <random>
#include "bw64/bw64.hpp"

using namespace bw64;

TEST_CASE("read_file_not_found") {
  REQUIRE_THROWS_AS(readFile("file_not_found.wav"), std::runtime_error);
}

TEST_CASE("read_rect_16bit") {
  auto bw64File = readFile("rect_16bit.wav");
  REQUIRE(bw64File->formatTag() == 1u);
  REQUIRE(bw64File->bitDepth() == 16u);
  REQUIRE(bw64File->sampleRate() == 44100u);
  REQUIRE(bw64File->channels() == 2u);
  REQUIRE(bw64File->numberOfFrames() == 22050u);
}

TEST_CASE("read_rect_24bit") {
  auto bw64File = readFile("rect_24bit.wav");
  REQUIRE(bw64File->formatTag() == 1u);
  REQUIRE(bw64File->bitDepth() == 24u);
  REQUIRE(bw64File->sampleRate() == 44100u);
  REQUIRE(bw64File->channels() == 2u);
  REQUIRE(bw64File->numberOfFrames() == 22050u);
}

TEST_CASE("read_rect_32bit") {
  auto bw64File = readFile("rect_32bit.wav");
  REQUIRE(bw64File->formatTag() == 1u);
  REQUIRE(bw64File->bitDepth() == 32u);
  REQUIRE(bw64File->sampleRate() == 44100u);
  REQUIRE(bw64File->channels() == 2u);
  REQUIRE(bw64File->numberOfFrames() == 22050u);
}

TEST_CASE("read_rect_24bit_rf64") {
  auto bw64File = readFile("rect_24bit_rf64.wav");
  REQUIRE(bw64File->formatTag() == 1u);
  REQUIRE(bw64File->bitDepth() == 24u);
  REQUIRE(bw64File->sampleRate() == 44100u);
  REQUIRE(bw64File->channels() == 2u);
  REQUIRE(bw64File->numberOfFrames() == 22050u);
}

TEST_CASE("read_rect_24bit_noriff") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_noriff.wav"), std::runtime_error);
}

TEST_CASE("read_rect_24bit_nowave") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_nowave.wav"), std::runtime_error);
}

TEST_CASE("read_rect_24bit_wrong_fmt_size") {
  REQUIRE_THROWS_AS(Bw64Reader("rect_24bit_wrong_fmt_size.wav"),
                    std::runtime_error);
}

TEST_CASE("read_noise_24bit_uneven_data_chunk_size") {
  auto bw64File = readFile("noise_24bit_uneven_data_chunk_size.wav");
  REQUIRE(bw64File->bitDepth() == 24);
  REQUIRE(bw64File->sampleRate() == 44100);
  REQUIRE(bw64File->channels() == 1);
  REQUIRE(bw64File->numberOfFrames() == 13);
  REQUIRE(bw64File->hasChunk(utils::fourCC("chna")) == true);
  REQUIRE(bw64File->chnaChunk() != nullptr);
  REQUIRE(bw64File->hasChunk(utils::fourCC("axml")) == false);
  REQUIRE(bw64File->axmlChunk() == nullptr);
}

TEST_CASE("read_check_chunks") {
  auto bw64File = readFile("rect_16bit.wav");
  REQUIRE(bw64File->chunks().size() == 2);
  REQUIRE(utils::fourCCToStr(bw64File->chunks().at(0).id) == "fmt ");
  REQUIRE(utils::fourCCToStr(bw64File->chunks().at(1).id) == "data");
  REQUIRE(bw64File->hasChunk(utils::fourCC("fmt ")) == true);
  REQUIRE(bw64File->hasChunk(utils::fourCC("chna")) == false);
  REQUIRE(bw64File->hasChunk(utils::fourCC("axml")) == false);
}

TEST_CASE("read_seek_tell") {
  auto bw64File = readFile("rect_16bit.wav");
  // should be positioned at the beginning after opening
  REQUIRE(bw64File->tell() == 0);

  // std::ios::beg
  bw64File->seek(INT32_MIN);
  REQUIRE(bw64File->tell() == 0);
  bw64File->seek(11025);
  REQUIRE(bw64File->tell() == 11025);
  bw64File->seek(22060);
  REQUIRE(bw64File->tell() == 22050);

  // std::ios::cur
  bw64File->seek(INT32_MIN, std::ios::cur);
  REQUIRE(bw64File->tell() == 0);
  bw64File->seek(11025, std::ios::cur);
  REQUIRE(bw64File->tell() == 11025);
  bw64File->seek(INT32_MAX, std::ios::cur);
  REQUIRE(bw64File->tell() == 22050);

  // std::ios::end
  bw64File->seek(INT32_MIN, std::ios::end);
  REQUIRE(bw64File->tell() == 0);
  bw64File->seek(-11025, std::ios::end);
  REQUIRE(bw64File->tell() == 11025);
  bw64File->seek(INT32_MAX, std::ios::end);
  REQUIRE(bw64File->tell() == 22050);
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
                  uint32_t sampleRate = 48000u) {
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

void writeRandom(const std::string& filename, uint16_t bitDepth,
                 uint64_t frames, uint16_t channels = 1u,
                 uint32_t sampleRate = 48000u) {
  // Generate vector with random values between -1.f and 1.f
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_real_distribution<float> dist(-1.f, 1.f);
  auto gen = [&]() { return dist(engine); };
  std::vector<float> data(frames * channels);
  generate(begin(data), end(data), gen);

  auto bw64File = writeFile(filename, channels, sampleRate, bitDepth);
  bw64File->write(&data[0], frames);
}

TEST_CASE("write_read_riff_header") {
  int frames = 4800;
  writeRandom("write_read_riff_header.wav", 32, frames);
  auto bw64File = readFile("write_read_riff_header.wav");
  REQUIRE(bw64File->fileFormat() == utils::fourCC("RIFF"));
  /**
   * 'WAVE' chunk header  :      4 bytes
   * 'JUNK' chunk header  :      8 bytes
   * 'JUNK' chunk payload :     28 bytes
   * 'fmt ' chunk header  :      8 bytes
   * 'fmt ' chunk payload :     16 bytes
   * 'chna' chunk header  :      8 bytes
   * 'chna' chunk payload : 40.964 bytes (1024 * 40 bytes per UID + 4 bytes
   * (numTracks/numUIDs) 'data' chunk header  :      8 bytes 'data' chunk
   * payload : 19.200 bytes (2 channels * 2 bytes per samples * 4800 frames)
   * --------------------------------------------------------------------------
   *                        60244 bytes
   */
  REQUIRE(bw64File->fileSize() == 60244);
}

TEST_CASE("write_read_96000") {
  int frames = 9600;
  writeRandom("write_read_96000.wav", 32, frames, 1u, 96000u);
  auto bw64File = readFile("write_read_96000.wav");
  REQUIRE(bw64File->sampleRate() == 96000u);
}

TEST_CASE("can_read_all_frames") {
  int frames = 13;
  auto bw64File = readFile("noise_24bit_uneven_data_chunk_size.wav");
  REQUIRE(bw64File->numberOfFrames() == 13);
  std::vector<float> data(frames * bw64File->channels(), 0.f);
  int readSampleCount = bw64File->read(&data[0], frames);
  REQUIRE(readSampleCount == 13);
}
