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
