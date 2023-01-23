/**
 * @file utils.hpp
 *
 * Collection of helper functions.
 */
#pragma once
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <memory>
#include <type_traits>
#include <stdint.h>
#include "chunks.hpp"

namespace bw64 {
  namespace utils {

    /// @brief Convert char array chunkIds to uint32_t
    inline constexpr uint32_t fourCC(char const p[5]) {
      return (p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0];
    }
    /// @brief Convert uint32_t chunkId to string
    inline std::string fourCCToStr(uint32_t value) {
      return std::string(reinterpret_cast<char*>(&value), 4);
    }

    /// @brief Read a value from a stream
    template <typename T>
    void readValue(std::istream& stream, T& dest) {
      stream.read(reinterpret_cast<char*>(&dest), sizeof(dest));
      if (stream.eof())
        throw std::runtime_error("file ended while reading value");
      if (!stream.good())
        throw std::runtime_error("file error while reading value");
    }

    /// @brief Read size bytes from stream into dest
    ///
    /// Dest may be null if size == 0. EOF and stream errors are checked.
    inline void readChunk(std::istream& stream, char* dest, size_t size) {
      if (size) {
        stream.read(dest, size);
        if (stream.eof())
          throw std::runtime_error("file ended while reading chunk");
        if (!stream.good())
          throw std::runtime_error("file error while reading chunk");
      }
    }

    /// @brief Write a value to a stream
    template <typename T,
              typename std::enable_if<
                  std::is_integral<typename std::remove_extent<T>::type>::value,
                  int>::type = 0>
    void writeValue(std::ostream& stream, const T& src) {
      stream.write(reinterpret_cast<const char*>(&src), sizeof(src));
    }

    template <typename ChunkType>
    void writeChunk(std::ostream& stream, std::shared_ptr<ChunkType> chunk,
                    uint32_t chunkSizeForHeader) {
      writeValue(stream, chunk->id());
      writeValue(stream, chunkSizeForHeader);
      if (chunk->id() != fourCC("data")) {
        chunk->write(stream);
        if (chunk->size() % 2 == 1) {
          writeValue(stream, '\0');
        }
      }
    }

    inline void writeChunkPlaceholder(std::ostream& stream, uint32_t id,
                                      uint32_t size) {
      utils::writeValue(stream, id);
      utils::writeValue(stream, size);
      for (unsigned int i = 0; i < size; ++i) {
        writeValue(stream, '\0');
      }
    }

    /// @brief Limit sample to [-1,+1]
    template <typename T, typename std::enable_if<
                              std::is_floating_point<T>::value, int>::type = 0>
    T clipSample(T value) {
      if (value > 1.f) {
        return 1.f;
      }
      if (value < -1.f) {
        return -1.f;
      }
      return value;
    }

    /// scale factor for converting between floats and ints
    template <typename T, int bits>
    constexpr T scaleFactor() {
      return static_cast<T>(static_cast<uint32_t>(1) << (bits - 1));
    }

    /// encode one sample to PCM
    template <int bytes, typename IntT, typename T,
              typename std::enable_if<std::is_floating_point<T>::value,
                                      int>::type = 0>
    void encode(T value, char* buffer) {
      static_assert(sizeof(IntT) >= bytes, "IntT must be larger than bytes");
      constexpr int bits = bytes * 8;
      using UnsignedT = typename std::make_unsigned<IntT>::type;

      value *= scaleFactor<T, bits>();

      constexpr IntT maxval =
          static_cast<IntT>((UnsignedT{1} << (bits - 1)) - 1);
      constexpr IntT minval = -maxval - 1;

      // clip or convert to int
      IntT value_int;
      if (value >= static_cast<T>(maxval))
        value_int = maxval;
      else if (value <= static_cast<T>(minval))
        value_int = minval;
      else
        value_int = static_cast<IntT>(std::lrint(value));

      for (size_t i = 0; i < bytes; i++)
        buffer[i] = (value_int >> (8 * i)) & 0xff;
    }

    /// decode one sample from PCM
    template <int bytes, typename IntT, typename T,
              typename std::enable_if<std::is_floating_point<T>::value,
                                      int>::type = 0>
    T decode(const char* buffer) {
      static_assert(sizeof(IntT) >= bytes, "IntT must be larger than bytes");
      constexpr int bits = bytes * 8;

      // when converting from a char to an int, sign extension occurs and the
      // high bit is replicated to the high bytes
      //
      // we don't want this for the low bytes (since the high bit of these is
      // not a sign bit), but we do for the high byte. for example when reading
      // 24 bits into an int32, the high byte needs to be filled with the sign
      // bit
      //
      // an explicit cast to IntT is used because int can technically be 16 bits
      IntT value = 0;
      for (size_t i = 0; i < (bytes - 1); i++)
        value |= (static_cast<IntT>(buffer[i]) & 0xff) << (i * 8);
      value |= static_cast<IntT>(buffer[bytes - 1]) << ((bytes - 1) * 8);

      constexpr T scale_inv = T{1} / scaleFactor<T, bits>();

      return clipSample(scale_inv * value);
    }

    /// @brief Decode (integer) PCM samples as float from char array
    template <typename T, typename std::enable_if<
                              std::is_floating_point<T>::value, int>::type = 0>
    void decodePcmSamples(const char* inBuffer, T* outBuffer,
                          uint64_t numberOfSamples, uint16_t bitsPerSample) {
      if (bitsPerSample == 16) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          outBuffer[i] = decode<2, int16_t, T>(inBuffer + i * 2);
        }
      } else if (bitsPerSample == 24) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          outBuffer[i] = decode<3, int32_t, T>(inBuffer + i * 3);
        }
      } else if (bitsPerSample == 32) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          outBuffer[i] = decode<4, int32_t, T>(inBuffer + i * 4);
        }
      } else {
        std::stringstream errorString;
        errorString << "unsupported number of bits: " << bitsPerSample;
        throw std::runtime_error(errorString.str());
      }
    }

    /// @brief Encode PCM samples from float array to char array
    template <typename T,
              typename = std::enable_if<std::is_floating_point<T>::value>>
    void encodePcmSamples(const T* inBuffer, char* outBuffer,
                          uint64_t numberOfSamples, uint16_t bitsPerSample) {
      if (bitsPerSample == 16) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          encode<2, int16_t>(inBuffer[i], outBuffer + 2 * i);
        }
      } else if (bitsPerSample == 24) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          encode<3, int32_t>(inBuffer[i], outBuffer + 3 * i);
        }
      } else if (bitsPerSample == 32) {
        for (uint64_t i = 0; i < numberOfSamples; ++i) {
          // work in doubles for 32 bit to avoid roundoff
          encode<4, int32_t>(static_cast<double>(inBuffer[i]),
                             outBuffer + 4 * i);
        }
      } else {
        std::stringstream errorString;
        errorString << "unsupported number of bits: " << bitsPerSample;
        throw std::runtime_error(errorString.str());
      }
    }

    /// check x against the maximum value that To can hold
    template <typename To, typename From>
    void checkUpper(From x) {
      using FromUS = typename std::make_unsigned<From>::type;
      using ToUS = typename std::make_unsigned<To>::type;
      // additional parenthesis around numeric_limits are for preventing
      // conflict with max(a,b) macro on windows without requiring user to
      // define NOMINMAX

      // only need to do this check if From can hold values bigger than To
      //
      // regular promotion works fine here, but MSVC complains about
      // signed/unsigned comparison
      if (static_cast<FromUS>((std::numeric_limits<From>::max)()) >
          static_cast<ToUS>((std::numeric_limits<To>::max)())) {
        if (x > static_cast<From>((std::numeric_limits<To>::max)()))
          throw std::runtime_error("overflow");
      }
    }

    /// when converting x from From to To, do we need to check that x is not
    /// below the lower bound of To?
    ///
    /// templated to avoid erronous warnings on MSVC, and not use enable_if
    template <typename To, typename From>
    struct NeedToCheckLower {
      using FromS = typename std::make_signed<From>::type;
      using ToS = typename std::make_signed<To>::type;

      // convert limits to signed, otherwise this comparison can be promoted to
      // unsigned
      static constexpr bool value =
          static_cast<FromS>((std::numeric_limits<From>::min)()) <
          static_cast<ToS>((std::numeric_limits<To>::min)());
    };

    /// check x against the minimum value that To can hold
    template <typename To, typename From>
    typename std::enable_if<NeedToCheckLower<To, From>::value>::type checkLower(
        From x) {
      using FromS = typename std::make_signed<From>::type;

      // - From is signed
      // - To may be unsigned
      //   - if it's unsigned, this is ok because the lower limit of to is 0
      //   - if it's signed, it's ok because From is larger than To
      if (x < static_cast<FromS>((std::numeric_limits<To>::min)()))
        throw std::runtime_error("underflow");
    }

    template <typename To, typename From>
    typename std::enable_if<!NeedToCheckLower<To, From>::value>::type
    checkLower(From) {}

    /// convert signed or unsigned integer x to To, checking for overflow and
    /// underflow
    template <typename To, typename From>
    To safeCast(From x) {
      checkUpper<To>(x);
      checkLower<To>(x);
      return static_cast<To>(x);
    }

    /// add x and y, checking for overflow and underflow. X and Y most both be
    /// representable by T; use safeCast to make sure
    template <typename T>
    T safeAdd(T x, T y) {
      if (((y > 0) && (x > ((std::numeric_limits<T>::max)() - y))) ||
          ((y < 0) && (x < ((std::numeric_limits<T>::min)() - y))))
        throw std::runtime_error(y > 0 ? "overflow" : "underflow");
      return x + y;
    }
  }  // namespace utils
}  // namespace bw64
