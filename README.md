# libbw64 – ITU-R BS.2088 Library

[![Build Status](https://travis-ci.org/IRT-Open-Source/libbw64.svg?branch=master)](https://travis-ci.org/IRT-Open-Source/libbw64)
[![Build status](https://ci.appveyor.com/api/projects/status/22p267utk363c84i/branch/master?svg=true)](https://ci.appveyor.com/project/opensourceirt/libbw64/branch/master)
[![Documentation Status](https://readthedocs.org/projects/libbw64/badge/?version=latest)](https://libbw64.readthedocs.io/en/latest/?badge=latest)

## Introduction

The `libbw64` library is a lightweight C++ header only library to read and write
BW64 files. BW64 is standardised as [Recommendation ITU-R
BS.2088](https://www.itu.int/rec/R-REC-BS.2088/en) and the successor of RF64. So
it already contains necessary extensions to support files which are bigger than
4 GB. Apart from that a BW64 file is able to contain the ADM metadata and link
it with the audio tracks in the file. To do that a BW64 file specifies two new
RIFF chunks – the `axml` chunk and the `chna` chunk. To parse, create, modify
and write the ADM metadata in the `axml` chunk you may use the
[`libadm`](https://github.com/irt-open-source/libadm) library.

### Features

- no dependencies
- support file sizes bigger than 4 GB (`ds64` chunk)
- read and write `axml` and `chna` chunks
- 16, 24, and 32 bit integer file formats

## Installation

### Manual

Just copy the content of the include directory to your project or add the
repository as a Git submodule to your project.

Alternatively the Git repository can be cloned and the library can be installed
system wide using the used CMake build system (version 3.5 or later). See the
following instructions for *nix systems.

```
git clone git@github.com:irt-open-source/libbw64.git
cd libbw64
mkdir build && cd build
cmake ..
make
make install
```

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

## Acknowledgement

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.

## License

```
Copyright 2018 Institut fuer Rundfunktechnik GmbH (IRT)

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
