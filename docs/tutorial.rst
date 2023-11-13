.. tutorial:

Tutorial
########

In this tutorial we will create a simple application which adjusts the level of
all channels in a BW64 file and writes the output to a new file. We assume that
the ``include`` path of the library is added to the ``PATH``.

Basic structure
---------------

Let us start with the basic structure of our programme.

.. code-block:: cpp

    #include <iostream>
    #include <bw64/bw64.hpp>

    const unsigned int BLOCK_SIZE = 4096;

    int main(int argc, char const* argv[]) {
      if (argc != 2) {
        std::cout << "usage: " << argv[0] << " [INFILE]" << std::endl;
        exit(1);
      }
      auto inFile = bw64::readFile(argv[1]);
      std::vector<float> buffer(BLOCK_SIZE * inFile->channels());
      while (!inFile->eof()) {
        auto readFrames = inFile->read(&buffer[0], BLOCK_SIZE);
        // TODO: process samples
      }
      return 0;
    }

We include the header and open the file we want to read using the
:cpp:func:`bw64::readFile()` function and add a ``while`` loop in which we read
the samples in a block buffer. The :cpp:func:`bw64::Bw64Reader::read()` expects
a float array and the number of frames, the function should try to read. One
frame contains one sample for each channel. So if the
:cpp:func:`bw64::Bw64Reader::read()` function should try to read ``N`` frames,
the buffer must be at least ``N * CHANNELS`` big. The samples are written into
the buffer in a channel interleaved order, as illustrated in the following
table.

+---------+-+-+-+-+-+-+-+-+-+-+--+
| Index   |0|1|2|3|4|5|6|7|8|9|10|
+---------+-+-+-+-+-+-+-+-+-+-+--+
| Channel |0|1|0|1|0|1|0|1|0|1|0 |
+---------+-+-+-+-+-+-+-+-+-+-+--+
| Sample  |0|0|1|1|2|2|3|3|4|4|5 |
+---------+-+-+-+-+-+-+-+-+-+-+--+

Note that we don't need to close our file at the end of the programme. This will
be done automatically when ``inFile`` is destroyed at the end of the programme.
In real applications :cpp:func:`bw64::Bw64Reader::close()` should be calles
before destruction to be able to properly catch exceptions.

Write files
-----------

As a next step we also prepare our output file to write the samples.

.. code-block:: cpp

    #include <iostream>
    #include <bw64/bw64.hpp>

    const unsigned int BLOCK_SIZE = 4096;

    int main(int argc, char const* argv[]) {
      if (argc != 3) {
        std::cout << "usage: " << argv[0] << " [INFILE] [OUTFILE]" << std::endl;
        exit(1);
      }
      auto inFile = bw64::readFile(argv[1]);
      auto outFile =
          bw64::writeFile(argv[2], inFile->channels(), inFile->sampleRate(),
                          inFile->bitDepth(), inFile->chnaChunk(), inFile->axmlChunk());

      std::vector<float> buffer(BLOCK_SIZE * inFile->channels());
      while (!inFile->eof()) {
        auto readFrames = inFile->read(&buffer[0], BLOCK_SIZE);
        // TODO: process samples
        outFile->write(&buffer[0], readFrames);
      }
      return 0;
    }

We use the information from the input file we opened to initialize our output
file. We also need to add the ``chna`` and ``axml`` chunks from the input file
to the output file during initialization. We can directly use the buffer we
passed to the :cpp:func:`bw64::Bw64Reader::read()` in the
:cpp:func:`bw64::Bw64Writer::write()` function to write the unmodified samples.
So also the :cpp:func:`bw64::Bw64Writer::write()` expects the order of the
samples to be interleaved as described above.

As with reading, the file will be automatically closed during destruction, but
in real applications :cpp:func:`bw64::Bw64Writer::close()` should be called
before destruction to be able to properly catch exceptions.

Add signal processing
---------------------

To make our example complete, let us add some basic signal processing and adjust
the gain of all channels.

.. code-block:: cpp

    #include <iostream>
    #include <algorithm>
    #include <functional>
    #include <bw64/bw64.hpp>

    const unsigned int BLOCK_SIZE = 4096;

    int main(int argc, char const* argv[]) {
      if (argc != 4) {
        std::cout << "usage: " << argv[0] << " [INFILE] [OUTFILE] [GAIN]"
                  << std::endl;
        exit(1);
      }
      auto inFile = bw64::readFile(argv[1]);
      auto outFile =
          bw64::writeFile(argv[2], inFile->channels(), inFile->sampleRate(),
                          inFile->bitDepth(), inFile->chnaChunk(), inFile->axmlChunk());

      std::vector<float> buffer(BLOCK_SIZE * inFile->channels());
      float gain = atof(argv[3]);
      while (!inFile->eof()) {
        auto readFrames = inFile->read(&buffer[0], BLOCK_SIZE);
        std::transform(buffer.begin(), buffer.end(), buffer.begin(),
                      [gain](float value) { return value * gain; });
        outFile->write(&buffer[0], readFrames);
      }
      return 0;
    }

