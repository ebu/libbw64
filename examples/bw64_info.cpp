#include <iostream>
#include <bw64/bw64.hpp>

using namespace bw64;

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [BW64_FILE]" << std::endl;
    exit(1);
  }
  auto bw64File = readFile(argv[1]);

  std::cout << "Format:" << std::endl;
  std::cout << " - formatTag: " << bw64File->formatTag() << std::endl;
  std::cout << " - channels: " << bw64File->channels() << std::endl;
  std::cout << " - sampleRate: " << bw64File->sampleRate() << std::endl;
  std::cout << " - bitDepth: " << bw64File->bitDepth() << std::endl;
  std::cout << " - numerOfFrames: " << bw64File->numberOfFrames() << std::endl;

  std::cout << "chunkIds:" << std::endl;
  for (auto& chunk : bw64File->chunks()) {
    std::cout << " - " << '\'' << utils::fourCCToStr(chunk.id) << '\''
              << std::endl;
  }

  if (bw64File->hasChunk(utils::fourCC("chna"))) {
    if (auto chnaChunk = bw64File->chnaChunk()) {
      std::cout << "ChnaChunk:" << std::endl;
      std::cout << " - numTracks: " << chnaChunk->numTracks() << std::endl;
      std::cout << " - numUids: " << chnaChunk->numUids() << std::endl;
      std::cout << " - audioIds:" << std::endl;
      for (auto audioId : chnaChunk->audioIds()) {
        std::cout << "   - ";
        std::cout << audioId.trackIndex() << ", " << audioId.uid() << ", "
                  << audioId.trackRef() << ", " << audioId.packRef()
                  << std::endl;
      }
    }
  }
  return 0;
}
