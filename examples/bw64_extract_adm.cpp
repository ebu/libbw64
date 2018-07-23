#include <iostream>
#include <bw64/bw64.hpp>

using namespace bw64;

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [BW64_FILE]" << std::endl;
    exit(1);
  }
  auto bw64File = readFile(argv[1]);
  if (bw64File->axmlChunk()) {
    std::stringstream axmlStringstream;
    bw64File->axmlChunk()->write(axmlStringstream);
    std::cout << axmlStringstream.str();
  } else {
    std::cerr << "could not find an axml chunk";
    exit(1);
  }
  return 0;
}
