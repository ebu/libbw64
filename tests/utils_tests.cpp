#include "bw64/bw64.hpp"

#define BOOST_TEST_MODULE UtilsTests
#include <boost/test/included/unit_test.hpp>

using namespace bw64;

BOOST_AUTO_TEST_CASE(decode_pcm_samples_8bit) {
  float decodedSamples[5];
  const char* encoded8bit = "\x00\x7f\x81";
  BOOST_CHECK_THROW(utils::decodePcmSamples(encoded8bit, decodedSamples, 5, 8),
                    std::runtime_error);
}

BOOST_AUTO_TEST_CASE(decode_pcm_samples_16bit) {
  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  float decodedSamples[5];

  const char* encoded16bit =
      "\x00\x00"
      "\xff\x7f"
      "\x01\x80"
      "\xff\x3f"
      "\x01\xc0";
  utils::decodePcmSamples(encoded16bit, decodedSamples, 5, 16);

  BOOST_CHECK_CLOSE(samples[0], decodedSamples[0], 1e-2);
  BOOST_CHECK_CLOSE(samples[1], decodedSamples[1], 1e-2);
  BOOST_CHECK_CLOSE(samples[2], decodedSamples[2], 1e-2);
  BOOST_CHECK_CLOSE(samples[3], decodedSamples[3], 1e-2);
  BOOST_CHECK_CLOSE(samples[4], decodedSamples[4], 1e-2);
}

BOOST_AUTO_TEST_CASE(decode_pcm_samples_24bit) {
  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  float decodedSamples[5];

  const char* encoded24bit =
      "\x00\x00\x00"
      "\xff\xff\x7f"
      "\x01\x00\x80"
      "\xff\xff\x3f"
      "\x01\x00\xc0";
  utils::decodePcmSamples(encoded24bit, decodedSamples, 5, 24);

  BOOST_CHECK_CLOSE(samples[0], decodedSamples[0], 1e-3);
  BOOST_CHECK_CLOSE(samples[1], decodedSamples[1], 1e-3);
  BOOST_CHECK_CLOSE(samples[2], decodedSamples[2], 1e-3);
  BOOST_CHECK_CLOSE(samples[3], decodedSamples[3], 1e-3);
  BOOST_CHECK_CLOSE(samples[4], decodedSamples[4], 1e-3);
}

BOOST_AUTO_TEST_CASE(decode_pcm_samples_32bit) {
  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  float decodedSamples[5];

  const char* encoded32bit =
      "\x00\x00\x00\x00"
      "\xff\xff\xff\x7f"
      "\x01\x00\x00\x80"
      "\xff\xff\xff\x3f"
      "\x01\x00\x00\xc0";
  utils::decodePcmSamples(encoded32bit, decodedSamples, 5, 32);

  BOOST_CHECK_CLOSE(samples[0], decodedSamples[0], 1e-6);
  BOOST_CHECK_CLOSE(samples[1], decodedSamples[1], 1e-6);
  BOOST_CHECK_CLOSE(samples[2], decodedSamples[2], 1e-6);
  BOOST_CHECK_CLOSE(samples[3], decodedSamples[3], 1e-6);
  BOOST_CHECK_CLOSE(samples[4], decodedSamples[4], 1e-6);
}

BOOST_AUTO_TEST_CASE(encode_pcm_samples_8bit) {
  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  char encoded8bit[5];

  BOOST_CHECK_THROW(utils::encodePcmSamples(samples, encoded8bit, 5, 8),
                    std::runtime_error);
}

BOOST_AUTO_TEST_CASE(encode_pcm_samples_16bit) {
  const char* encodedSamples =
      "\x00\x00"
      "\xff\x7f"
      "\x01\x80"
      "\xff\x3f"
      "\x01\xc0";
  char encoded16bit[10];

  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  utils::encodePcmSamples(samples, encoded16bit, 5, 16);

  for (int byte = 0; byte < 10; ++byte) {
    BOOST_TEST(encodedSamples[byte] == encoded16bit[byte]);
  }
}

BOOST_AUTO_TEST_CASE(encode_pcm_samples_24bit) {
  const char* encodedSamples =
      "\x00\x00\x00"
      "\xff\xff\x7f"
      "\x01\x00\x80"
      "\xff\xff\x3f"
      "\x01\x00\xc0";
  char encoded24bit[15];

  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  utils::encodePcmSamples(samples, encoded24bit, 5, 24);

  for (int byte = 0; byte < 15; ++byte) {
    BOOST_TEST(encodedSamples[byte] == encoded24bit[byte]);
  }
}

BOOST_AUTO_TEST_CASE(encode_pcm_samples_32bit) {
  const char* encodedSamples =
      "\x00\x00\x00\x00"
      "\xff\xff\xff\x7f"
      "\x01\x00\x00\x80"
      "\xff\xff\xff\x3f"
      "\x01\x00\x00\xc0";
  char encoded32bit[20];

  const float samples[] = {0.0, 1.0, -1.0, 0.5, -0.5};
  utils::encodePcmSamples(samples, encoded32bit, 5, 32);

  for (int byte = 0; byte < 20; ++byte) {
    BOOST_TEST(encodedSamples[byte] == encoded32bit[byte]);
  }
}

BOOST_AUTO_TEST_CASE(write_chunk_with_padding) {
  auto axmlChunk = std::make_shared<AxmlChunk>("123456789");
  std::ostringstream stream;
  utils::writeChunk(stream, axmlChunk,
                    static_cast<uint32_t>(axmlChunk->size()));
  unsigned int headerSize = 8u;
  unsigned int padding = 1u;
  BOOST_TEST(static_cast<uint64_t>(stream.tellp()) ==
             headerSize + axmlChunk->size() + padding);
}

BOOST_AUTO_TEST_CASE(write_chunk_without_padding) {
  auto axmlChunk = std::make_shared<AxmlChunk>("1234567890");
  std::ostringstream stream;
  utils::writeChunk(stream, axmlChunk,
                    static_cast<uint32_t>(axmlChunk->size()));
  unsigned int headerSize = 8u;
  unsigned int padding = 0u;
  BOOST_TEST(static_cast<uint64_t>(stream.tellp()) ==
             headerSize + axmlChunk->size() + padding);
}
