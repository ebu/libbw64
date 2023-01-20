# libbw64 – ITU-R BS.2088 Library

[![](https://github.com/ebu/libbw64/workflows/Linux/badge.svg)](https://github.com/ebu/libbw64/actions?workflow=Linux)
[![](https://github.com/ebu/libbw64/workflows/macOS/badge.svg)](https://github.com/ebu/libbw64/actions?workflow=macOS)
[![](https://github.com/ebu/libbw64/workflows/Windows/badge.svg)](https://github.com/ebu/libbw64/actions?workflow=Windows)
[![Documentation Status](https://readthedocs.org/projects/libbw64/badge/?version=latest)](https://libbw64.readthedocs.io/en/latest/?badge=latest)
[![codecov](https://codecov.io/gh/ebu/libbw64/branch/master/graph/badge.svg)](https://codecov.io/gh/ebu/libbw64)

## Introduction

The `libbw64` library is a lightweight C++ header only library to read and write
BW64 files. BW64 is standardised as [Recommendation ITU-R
BS.2088](https://www.itu.int/rec/R-REC-BS.2088/en) and the successor of RF64. So
it already contains necessary extensions to support files which are bigger than
4 GB. Apart from that a BW64 file is able to contain the ADM metadata and link
it with the audio tracks in the file. To do that a BW64 file specifies two new
RIFF chunks – the `axml` chunk and the `chna` chunk. To parse, create, modify
and write the ADM metadata in the `axml` chunk you may use the
[`libadm`](https://github.com/ebu/libadm) library.

[Read the documentation](https://libbw64.readthedocs.io/en/latest/) to get
started.

### Features

- no dependencies
- support file sizes bigger than 4 GB (`ds64` chunk)
- read and write `axml` and `chna` chunks
- 16, 24, and 32 bit integer file formats

## Installation

### macOS
On macOS you can use homebrew to install the library. You just have to add the
EBU NGA homebrew tap and can then use the usual install command.

```
brew tap ebu/homebrew-nga
brew install libbw64
```

### Manual installation
To manually install the library you have to clone the git repository and then
use the CMake (version 3.5 or later) build system to build and install it.

```
git clone git@github.com:ebu/libbw64.git
cd libbw64
mkdir build && cd build
cmake ..
make
make install
```

### Copy headers

The `libbw64` is a header-only library so installing the library is not by all
means necessary. It is also possible to just copy the content of the `include`
directory to your project and make sure to add the `bw64` folder to your
`PATH`, that the header files can be found by the compiler.

## CMake
As the library uses CMake as a build system it is really easy to set up and use
if your project does too. Assuming you have installed the library, the following
code shows a complete CMake example to compile a program which uses the
`libbw64`.

```
cmake_minimum_required(VERSION 3.5)
project(libbw64_example VERSION 1.0.0 LANGUAGES CXX)

find_package(bw64 REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE bw64)
```

If you prefer not to install the library on your system you can also use the
library as a subproject. You can just add the library as a CMake subproject.
Just add the folder containing the repository to your project and you can use
the `bw64` target.

```
cmake_minimum_required(VERSION 3.5)
project(libbw64_example VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(submodules/libbw64)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE bw64)
```

#### Note

If `libbw64` is used as a CMake subproject the default values of the options

- `BW64_UNIT_TESTS`
- `BW64_EXAMPLES`
- `BW64_PACKAGE_AND_INSTALL`

are automatically set to `FALSE`.

## Basic usage

To use `libbw64` simply include the main header file `bw64.hpp` and use the
functions `bw64::readFile` and `bw64::writeFile` to create a `BW64Reader` or
`BW64Writer` object. The functions return a `unique_ptr` to the object.

### Read a BW64 file

```cpp
#include <iostream>
#include <vector>
#include <bw64/bw64.hpp>

const unsigned int BLOCK_SIZE = 4096;

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [INFILE]" << std::endl;
    exit(1);
  }
  auto inFile = bw64::readFile(argv[1]);
  auto axmlChunk = inFile->axmlChunk();  // get axml chunk
  auto chnaChunk = inFile->chnaChunk();  // get chna chunk
  std::vector<float> buffer(BLOCK_SIZE * inFile->channels());
  while (!inFile->eof()) {
    auto readFrames = inFile->read(&buffer[0], BLOCK_SIZE);
    // TODO: process samples
  }
  return 0;
}
```

### Write a BW64 file

```cpp
#include <iostream>
#include <vector>
#include <bw64/bw64.hpp>

const unsigned int BLOCK_SIZE = 4096;

int main(int argc, char const* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " [OUTFILE]" << std::endl;
    exit(1);
  }
  auto outFile = bw64::writeFile(argv[1]);
  std::vector<float> buffer(BLOCK_SIZE);
  for (int i = 0; i < 100; ++i) {
    // TODO: fill vector with data
    outFile->write(&buffer.front(), BLOCK_SIZE);
  }
  return 0;
}
```

### More examples

See the [examples folder](examples) in the repository for more examples.

## Credits

*libbw64* is originally a development of the [IRT](https://www.irt.de).

## Acknowledgement

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.

## License

```
Copyright 2018-2020 The libbw64 Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
