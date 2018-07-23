#include <iostream>
#include <vector>
#include <bw64/bw64.hpp>

using namespace bw64;

const unsigned int BLOCK_SIZE = 4096;

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " [INFILE] [OUTFILE]" << std::endl;
    exit(1);
  }
  auto inFile = readFile(argv[1]);

  std::cout << "Format:" << std::endl;
  std::cout << " - formatTag: " << inFile->formatTag() << std::endl;
  std::cout << " - channels: " << inFile->channels() << std::endl;
  std::cout << " - sampleRate: " << inFile->sampleRate() << std::endl;
  std::cout << " - bitDepth: " << inFile->bitDepth() << std::endl;
  std::cout << " - numerOfFrames: " << inFile->numberOfFrames() << std::endl;

  auto outFile = writeFile(argv[2], inFile->channels(), inFile->sampleRate(),
                           inFile->bitDepth());

  std::vector<float> buffer(BLOCK_SIZE * inFile->channels());
  while (!inFile->eof()) {
    auto readFrames = inFile->read(&buffer[0], BLOCK_SIZE);
    outFile->write(&buffer[0], readFrames);
  }

  return 0;
}
