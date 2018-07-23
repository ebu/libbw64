#include <sstream>
#include "bw64/bw64.hpp"

#define BOOST_TEST_MODULE ChunkTests
#include <boost/test/included/unit_test.hpp>

using namespace bw64;

BOOST_AUTO_TEST_CASE(rect_16bit) {
  Bw64Reader bw64File("testfiles/rect_16bit.wav");
  BOOST_TEST(bw64File.bitDepth() == 16);
  BOOST_TEST(bw64File.sampleRate() == 44100);
  BOOST_TEST(bw64File.channels() == 2);
  BOOST_TEST(bw64File.numberOfFrames() == 22050);
}

BOOST_AUTO_TEST_CASE(rect_24bit) {
  Bw64Reader bw64File("testfiles/rect_24bit.wav");
  BOOST_TEST(bw64File.bitDepth() == 24);
  BOOST_TEST(bw64File.sampleRate() == 44100);
  BOOST_TEST(bw64File.channels() == 2);
  BOOST_TEST(bw64File.numberOfFrames() == 22050);
}

BOOST_AUTO_TEST_CASE(rect_32bit) {
  Bw64Reader bw64File("testfiles/rect_32bit.wav");
  BOOST_TEST(bw64File.bitDepth() == 32);
  BOOST_TEST(bw64File.sampleRate() == 44100);
  BOOST_TEST(bw64File.channels() == 2);
  BOOST_TEST(bw64File.numberOfFrames() == 22050);
}

BOOST_AUTO_TEST_CASE(rect_24bit_rf64) {
  Bw64Reader bw64File("testfiles/rect_24bit_rf64.wav");
  BOOST_TEST(bw64File.bitDepth() == 24);
  BOOST_TEST(bw64File.sampleRate() == 44100);
  BOOST_TEST(bw64File.channels() == 2);
  BOOST_TEST(bw64File.numberOfFrames() == 22050);
}

BOOST_AUTO_TEST_CASE(rect_24bit_noriff) {
  BOOST_CHECK_THROW(Bw64Reader("testfiles/rect_24bit_noriff.wav"),
                    std::runtime_error);
}

BOOST_AUTO_TEST_CASE(rect_24bit_nowave) {
  BOOST_CHECK_THROW(Bw64Reader("testfiles/rect_24bit_nowave.wav"),
                    std::runtime_error);
}

BOOST_AUTO_TEST_CASE(rect_24bit_wrong_fmt_size) {
  BOOST_CHECK_THROW(Bw64Reader("testfiles/rect_24bit_wrong_fmt_size.wav"),
                    std::runtime_error);
}

BOOST_AUTO_TEST_CASE(noise_24bit_uneven_data_chunk_size) {
  Bw64Reader bw64File("testfiles/noise_24bit_uneven_data_chunk_size.wav");
  BOOST_TEST(bw64File.bitDepth() == 24);
  BOOST_TEST(bw64File.sampleRate() == 44100);
  BOOST_TEST(bw64File.channels() == 1);
  BOOST_TEST(bw64File.numberOfFrames() == 13);
  BOOST_CHECK(bw64File.chnaChunk() != nullptr);
}
