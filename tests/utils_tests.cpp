#include <catch2/catch.hpp>
#include "bw64/bw64.hpp"

using namespace bw64;

TEST_CASE("fourCC") {
  REQUIRE(utils::fourCCToStr(utils::fourCC("RIFF")) == "RIFF");
  REQUIRE(utils::fourCCToStr(utils::fourCC("fmt ")) == "fmt ");
  REQUIRE(utils::fourCCToStr(utils::fourCC("chna")) == "chna");
  REQUIRE(utils::fourCCToStr(utils::fourCC("axml")) == "axml");
}

TEST_CASE("decode_pcm_samples_8bit") {
  float decodedSamples[5];
  const char* encoded8bit = "\x00\x7f\x81";
  REQUIRE_THROWS_AS(utils::decodePcmSamples(encoded8bit, decodedSamples, 5, 8),
                    std::runtime_error);
}

TEST_CASE("decode_pcm_samples_16bit") {
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[5];

  const char* encoded16bit =
      "\x00\x00"
      "\xff\x7f"
      "\x01\x80"
      "\xff\x3f"
      "\x01\xc0";
  utils::decodePcmSamples(encoded16bit, decodedSamples, 5, 16);

  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-2));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-2));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-2));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-2));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-2));
}

TEST_CASE("decode_pcm_samples_24bit") {
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[5];

  const char* encoded24bit =
      "\x00\x00\x00"
      "\xff\xff\x7f"
      "\x01\x00\x80"
      "\xff\xff\x3f"
      "\x01\x00\xc0";
  utils::decodePcmSamples(encoded24bit, decodedSamples, 5, 24);

  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-3));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-3));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-3));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-3));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-3));
}

TEST_CASE("decode_pcm_samples_32bit") {
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[5];

  const char* encoded32bit =
      "\x00\x00\x00\x00"
      "\xff\xff\xff\x7f"
      "\x01\x00\x00\x80"
      "\xff\xff\xff\x3f"
      "\x01\x00\x00\xc0";
  utils::decodePcmSamples(encoded32bit, decodedSamples, 5, 32);

  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-6));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-6));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-6));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-6));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-6));
}

TEST_CASE("encode_pcm_samples_8bit") {
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  char encoded8bit[5];

  REQUIRE_THROWS_AS(utils::encodePcmSamples(samples, encoded8bit, 5, 8),
                    std::runtime_error);
}

TEST_CASE("encode_pcm_samples_16bit") {
  const char* encodedSamples =
      "\x00\x00"
      "\xff\x7f"
      "\x01\x80"
      "\xff\x3f"
      "\x01\xc0";
  char encoded16bit[10];

  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  utils::encodePcmSamples(samples, encoded16bit, 5, 16);

  for (int byte = 0; byte < 10; ++byte) {
    REQUIRE(encodedSamples[byte] == encoded16bit[byte]);
  }
}

TEST_CASE("encode_pcm_samples_24bit") {
  const char* encodedSamples =
      "\x00\x00\x00"
      "\xff\xff\x7f"
      "\x01\x00\x80"
      "\xff\xff\x3f"
      "\x01\x00\xc0";
  char encoded24bit[15];

  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  utils::encodePcmSamples(samples, encoded24bit, 5, 24);

  for (int byte = 0; byte < 15; ++byte) {
    REQUIRE(encodedSamples[byte] == encoded24bit[byte]);
  }
}

TEST_CASE("encode_pcm_samples_32bit") {
  const char* encodedSamples =
      "\x00\x00\x00\x00"
      "\xff\xff\xff\x7f"
      "\x01\x00\x00\x80"
      "\xff\xff\xff\x3f"
      "\x01\x00\x00\xc0";
  char encoded32bit[20];

  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  utils::encodePcmSamples(samples, encoded32bit, 5, 32);

  for (int byte = 0; byte < 20; ++byte) {
    REQUIRE(encodedSamples[byte] == encoded32bit[byte]);
  }
}

TEST_CASE("encode_decode_pcm_samples_16bit") {
  char encoded16bit[10];
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[] = {0.f, 0.f, 0.f, 0.f, 0.f};
  utils::encodePcmSamples(samples, encoded16bit, 5, 16);
  utils::decodePcmSamples(encoded16bit, decodedSamples, 5, 16);
  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-2));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-2));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-2));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-2));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-2));
}

TEST_CASE("encode_decode_pcm_samples_24bit") {
  char encoded24bit[15];
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[] = {0.f, 0.f, 0.f, 0.f, 0.f};
  utils::encodePcmSamples(samples, encoded24bit, 5, 24);
  utils::decodePcmSamples(encoded24bit, decodedSamples, 5, 24);
  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-3));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-3));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-3));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-3));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-3));
}

TEST_CASE("encode_decode_pcm_samples_32bit") {
  char encoded32bit[20];
  const float samples[] = {0.f, 1.f, -1.f, 0.5f, -0.5f};
  float decodedSamples[] = {0.f, 0.f, 0.f, 0.f, 0.f};
  utils::encodePcmSamples(samples, encoded32bit, 5, 32);
  utils::decodePcmSamples(encoded32bit, decodedSamples, 5, 32);
  REQUIRE(samples[0] == Approx(decodedSamples[0]).epsilon(1e-6));
  REQUIRE(samples[1] == Approx(decodedSamples[1]).epsilon(1e-6));
  REQUIRE(samples[2] == Approx(decodedSamples[2]).epsilon(1e-6));
  REQUIRE(samples[3] == Approx(decodedSamples[3]).epsilon(1e-6));
  REQUIRE(samples[4] == Approx(decodedSamples[4]).epsilon(1e-6));
}

TEST_CASE("write_chunk_with_padding") {
  auto axmlChunk = std::make_shared<AxmlChunk>("123456789");
  std::ostringstream stream;
  utils::writeChunk(stream, axmlChunk,
                    static_cast<uint32_t>(axmlChunk->size()));
  unsigned int headerSize = 8u;
  unsigned int padding = 1u;
  REQUIRE(static_cast<uint64_t>(stream.tellp()) ==
          headerSize + axmlChunk->size() + padding);
}

TEST_CASE("write_chunk_without_padding") {
  auto axmlChunk = std::make_shared<AxmlChunk>("1234567890");
  std::ostringstream stream;
  utils::writeChunk(stream, axmlChunk,
                    static_cast<uint32_t>(axmlChunk->size()));
  unsigned int headerSize = 8u;
  unsigned int padding = 0u;
  REQUIRE(static_cast<uint64_t>(stream.tellp()) ==
          headerSize + axmlChunk->size() + padding);
}

template <typename T>
void checkSameType() {
  CHECK_NOTHROW(utils::safeCast<T>(std::numeric_limits<T>::max()));
  CHECK_NOTHROW(utils::safeCast<T>(static_cast<T>(0)));
  CHECK_NOTHROW(utils::safeCast<T>(std::numeric_limits<T>::min()));
}

// when positive range of U is greater than T
template <typename T, typename U>
void checkLargerTypePositive() {
  CHECK_NOTHROW(utils::safeCast<T>(static_cast<U>(0)));
  CHECK_NOTHROW(
      utils::safeCast<T>(static_cast<U>(std::numeric_limits<T>::max())));
  CHECK_THROWS_WITH(
      utils::safeCast<T>(static_cast<U>(std::numeric_limits<T>::max()) + 1),
      "overflow");
  CHECK_THROWS_WITH(utils::safeCast<T>(std::numeric_limits<U>::max()),
                    "overflow");

  CHECK_NOTHROW(utils::safeCast<U>(static_cast<T>(0)));
  CHECK_NOTHROW(utils::safeCast<U>(std::numeric_limits<T>::max()));
}

// when negative range of U is greater than T
template <typename T, typename U>
void checkLargerTypeNegative() {
  CHECK_NOTHROW(utils::safeCast<T>(static_cast<U>(0)));
  CHECK_NOTHROW(
      utils::safeCast<T>(static_cast<U>(std::numeric_limits<T>::min())));
  CHECK_THROWS_WITH(
      utils::safeCast<T>(static_cast<U>(std::numeric_limits<T>::min()) - 1),
      "underflow");
  CHECK_THROWS_WITH(utils::safeCast<T>(std::numeric_limits<U>::min()),
                    "underflow");

  CHECK_NOTHROW(utils::safeCast<U>(static_cast<T>(0)));
  CHECK_NOTHROW(utils::safeCast<U>(std::numeric_limits<T>::min()));
}

TEST_CASE("safe cast") {
  checkSameType<int32_t>();
  checkSameType<uint32_t>();
  checkSameType<int64_t>();
  checkSameType<uint64_t>();

  checkLargerTypePositive<int32_t, int64_t>();
  checkLargerTypePositive<int32_t, uint64_t>();
  checkLargerTypePositive<uint32_t, int64_t>();
  checkLargerTypePositive<uint32_t, uint64_t>();
  checkLargerTypePositive<int64_t, uint64_t>();

  checkLargerTypeNegative<uint32_t, int32_t>();
  checkLargerTypeNegative<uint32_t, int64_t>();
  checkLargerTypeNegative<uint64_t, int32_t>();
  checkLargerTypeNegative<uint64_t, int64_t>();
  checkLargerTypeNegative<int32_t, int64_t>();
}

template <typename T>
void checkAddPositive() {
  CHECK_NOTHROW(utils::safeAdd<T>(0, 0));
  CHECK_NOTHROW(utils::safeAdd<T>(0, std::numeric_limits<T>::max()));
  CHECK_NOTHROW(utils::safeAdd<T>(std::numeric_limits<T>::max(), 0));

  CHECK_NOTHROW(utils::safeAdd<T>(1, std::numeric_limits<T>::max() - 1));
  CHECK_NOTHROW(utils::safeAdd<T>(std::numeric_limits<T>::max() - 1, 1));

  CHECK_THROWS_WITH(utils::safeAdd<T>(1, std::numeric_limits<T>::max()),
                    "overflow");
  CHECK_THROWS_WITH(utils::safeAdd<T>(std::numeric_limits<T>::max(), 1),
                    "overflow");

  CHECK_THROWS_WITH(utils::safeAdd<T>(std::numeric_limits<T>::max(),
                                      std::numeric_limits<T>::max()),
                    "overflow");
}

template <typename T>
void checkAddNegative() {
  CHECK_NOTHROW(utils::safeAdd<T>(0, 0));
  CHECK_NOTHROW(utils::safeAdd<T>(0, std::numeric_limits<T>::min()));
  CHECK_NOTHROW(utils::safeAdd<T>(std::numeric_limits<T>::min(), 0));
  CHECK_NOTHROW(utils::safeAdd<T>(1, std::numeric_limits<T>::min()));
  CHECK_NOTHROW(utils::safeAdd<T>(std::numeric_limits<T>::min(), 1));

  CHECK_NOTHROW(utils::safeAdd<T>(-1, std::numeric_limits<T>::max()));
  CHECK_NOTHROW(utils::safeAdd<T>(std::numeric_limits<T>::max(), -1));

  CHECK_THROWS_WITH(utils::safeAdd<T>(-1, std::numeric_limits<T>::min()),
                    "underflow");
  CHECK_THROWS_WITH(utils::safeAdd<T>(std::numeric_limits<T>::min(), -1),
                    "underflow");

  CHECK_THROWS_WITH(utils::safeAdd<T>(std::numeric_limits<T>::min(),
                                      std::numeric_limits<T>::min()),
                    "underflow");
}

TEST_CASE("safe add") {
  checkAddPositive<uint32_t>();
  checkAddPositive<uint64_t>();
  checkAddPositive<int32_t>();
  checkAddPositive<int64_t>();

  checkAddNegative<int32_t>();
  checkAddNegative<int64_t>();
}
