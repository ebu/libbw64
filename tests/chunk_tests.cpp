#include <catch2/catch.hpp>
#include "bw64/bw64.hpp"
#include "bw64/parser.hpp"
#include "bw64/utils.hpp"

using namespace bw64;

TEST_CASE("format_info_chunk") {
  // basic test
  {
    const char* formatChunkByteArray =
        "\x01\x00\x01\x00"  // formatTag = 1; channelCount = 1
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x02\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    auto formatInfoChunk =
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16);
    REQUIRE(formatInfoChunk->formatTag() == 1);
    REQUIRE(formatInfoChunk->channelCount() == 1);
    REQUIRE(formatInfoChunk->sampleRate() == 48000);
    REQUIRE(formatInfoChunk->bytesPerSecond() == 96000);
    REQUIRE(formatInfoChunk->blockAlignment() == 2);
    REQUIRE(formatInfoChunk->bitsPerSample() == 16);
  }
  // wrong chunkSize
  {
    const char* formatChunkByteArray =
        "\x01\x00\x01\x00"  // formatTag = 1; channelCount = 1
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x02\x00\x10\x00"  // blockAlignment = 2; bitsPerSample = 16
        "\x00\x00\x00\x00";
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 22));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 22),
        std::runtime_error);
  }
  // illegal formatTag
  {
    const char* formatChunkByteArray =
        "\x02\x00\x01\x00"  // formatTag = 2; channelCount = 1
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x02\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16),
        std::runtime_error);
  }
  // wrong channelCount
  {
    const char* formatChunkByteArray =
        "\x01\x00\x00\x00"  // formatTag = 1; channelCount = 0
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x02\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16),
        std::runtime_error);
  }
  // wrong samplerate
  {
    const char* formatChunkByteArray =
        "\x01\x00\x01\x00"  // formatTag = 1; channelCount = 1
        "\x00\x00\x00\x00"  // sampleRate = 0
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x02\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16),
        std::runtime_error);
  }
  // wrong bytesPerSecond
  {
    const char* formatChunkByteArray =
        "\x01\x00\x01\x00"  // formatTag = 1; channelCount = 1
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x01\x77\x01\x00"  // bytesPerSecond = 96001
        "\x02\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16),
        std::runtime_error);
  }
  // wrong blockAlignment
  {
    const char* formatChunkByteArray =
        "\x01\x00\x01\x00"  // formatTag = 1; channelCount = 1
        "\x80\xbb\x00\x00"  // sampleRate = 48000
        "\x00\x77\x01\x00"  // bytesPerSecond = 96000
        "\x00\x00\x10\x00";  // blockAlignment = 2; bitsPerSample = 16
    std::istringstream formatChunkStream(std::string(formatChunkByteArray, 16));
    REQUIRE_THROWS_AS(
        parseFormatInfoChunk(formatChunkStream, utils::fourCC("fmt "), 16),
        std::runtime_error);
  }
  // read/write
  {
    std::stringstream stream;
    auto formatChunk = std::make_shared<FormatInfoChunk>(2, 48000, 24);
    formatChunk->write(stream);
    auto formatChunkReread =
        parseFormatInfoChunk(stream, utils::fourCC("fmt "), 16);
    REQUIRE(formatChunkReread->channelCount() == 2);
    REQUIRE(formatChunkReread->sampleRate() == 48000);
    REQUIRE(formatChunkReread->bitsPerSample() == 24);
  }
}

TEST_CASE("chna_chunk") {
  // basic test
  {
    const char* chnaChunkByteArray =
        "\x01\x00\x01\x00"  // numTracks = 1; numUids = 1
        "\x01\x00"  // trackIndex = 1
        "\x41\x54\x55\x5f\x30\x30\x30\x30\x30\x30\x30"  // ATU_00000001
        "\x31\x41\x54\x5f\x30\x30\x30\x33\x31\x30\x30\x31\x5f\x30"  // AT_00031001_01
        "\x31\x41\x50\x5f\x30\x30\x30\x33\x31\x30\x30\x31"  // AP_00031001
        "\x00";  // padding
    std::istringstream chnaChunkStream(std::string(chnaChunkByteArray, 52));
    auto chnaChunk = parseChnaChunk(chnaChunkStream, utils::fourCC("chna"), 44);
    REQUIRE(chnaChunk->numTracks() == 1);
    REQUIRE(chnaChunk->numUids() == 1);
    REQUIRE(chnaChunk->audioIds().size() == 1);
    REQUIRE(chnaChunk->audioIds()[0].trackIndex() == 1);
    REQUIRE(chnaChunk->audioIds()[0].uid() == "ATU_00000001");
    REQUIRE(chnaChunk->audioIds()[0].trackRef() == "AT_00031001_01");
    REQUIRE(chnaChunk->audioIds()[0].packRef() == "AP_00031001");
  }
  // read/write
  {
    std::stringstream stream;
    auto chnaChunk = std::make_shared<ChnaChunk>();
    chnaChunk->addAudioId(
        AudioId(1, "ATU_00000001", "AT_00031001_01", "AP_00031001"));
    chnaChunk->addAudioId(
        AudioId(1, "ATU_00000002", "AT_00031002_01", "AP_00031002"));
    chnaChunk->addAudioId(
        AudioId(2, "ATU_00000003", "AT_00031003_01", "AP_00031003"));
    chnaChunk->write(stream);

    std::cout << chnaChunk->numTracks() << std::endl;
    std::cout << chnaChunk->numUids() << std::endl;
    for (auto audioId : chnaChunk->audioIds()) {
      std::cout << audioId.trackIndex() << " " << audioId.uid() << " "
                << audioId.trackRef() << " " << audioId.packRef() << std::endl;
    }

    auto chnaChunkReread = parseChnaChunk(stream, utils::fourCC("chna"), 124);
    REQUIRE(chnaChunkReread->numTracks() == 2);
    REQUIRE(chnaChunkReread->numUids() == 3);
    REQUIRE(chnaChunkReread->audioIds()[0].trackIndex() == 1);
    REQUIRE(chnaChunkReread->audioIds()[0].uid() == "ATU_00000001");
    REQUIRE(chnaChunkReread->audioIds()[0].trackRef() == "AT_00031001_01");
    REQUIRE(chnaChunkReread->audioIds()[0].packRef() == "AP_00031001");
    REQUIRE(chnaChunkReread->audioIds()[1].trackIndex() == 1);
    REQUIRE(chnaChunkReread->audioIds()[1].uid() == "ATU_00000002");
    REQUIRE(chnaChunkReread->audioIds()[1].trackRef() == "AT_00031002_01");
    REQUIRE(chnaChunkReread->audioIds()[1].packRef() == "AP_00031002");
    REQUIRE(chnaChunkReread->audioIds()[2].trackIndex() == 2);
    REQUIRE(chnaChunkReread->audioIds()[2].uid() == "ATU_00000003");
    REQUIRE(chnaChunkReread->audioIds()[2].trackRef() == "AT_00031003_01");
    REQUIRE(chnaChunkReread->audioIds()[2].packRef() == "AP_00031003");
  }
}

TEST_CASE("ds64_chunk") {
  // basic test
  {
    const char* ds64ChunkByteArray =
        "\x9a\xc6\x22\x31\xa5\x00\x00\x00"  // bw64Size = 709493966490
        "\xa4\x25\x87\xcc\x86\x00\x00\x00"  // dataSize = 578957026724
        "\x00\x00\x00\x00\x00\x00\x00\x00"  // dummySize = 0
        "\x01\x00\x00\x00"  // tableSize = 1
        "\x61\x78\x6d\x6c"  // chunkId = "axml"
        "\x30\x5a\xc8\x00\x00\x00\x00\x00";  // axmlChunkSize = 48
    std::istringstream ds64ChunkStream(std::string(ds64ChunkByteArray, 40));
    auto ds64Chunk =
        parseDataSize64Chunk(ds64ChunkStream, utils::fourCC("ds64"), 40);
    REQUIRE(ds64Chunk->bw64Size() == 709493966490);
    REQUIRE(ds64Chunk->dataSize() == 578957026724);
    REQUIRE(ds64Chunk->dummySize() == 0);
    auto axmlId = utils::fourCC("axml");
    REQUIRE(ds64Chunk->getChunkSize(axmlId) == 13130288);
  }
  // read/write
  {
    std::stringstream stream;
    auto dataSize64Chunk =
        std::make_shared<DataSize64Chunk>(987654321, 123456789);
    auto axmlId = utils::fourCC("axml");
    dataSize64Chunk->setChunkSize(axmlId, 654321);
    dataSize64Chunk->write(stream);
    auto dataSize64ChunkReread =
        parseDataSize64Chunk(stream, utils::fourCC("ds64"), 40);
    REQUIRE(dataSize64ChunkReread->bw64Size() == 987654321);
    REQUIRE(dataSize64ChunkReread->dataSize() == 123456789);
    REQUIRE(dataSize64ChunkReread->tableLength() == 1);
    REQUIRE(dataSize64ChunkReread->getChunkSize(axmlId) == 654321);
  }
}
