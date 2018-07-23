.. reference:

Main functions
##############

:cpp:class:`bw64::Bw64Reader` and :cpp:class:`bw64::Bw64Writer` classes usually
should not be created manually. Instead the two builder functions to either read
or write a file should be used.

.. doxygenfunction:: bw64::readFile
.. doxygenfunction:: bw64::writeFile

BW64 file classes
#################

.. doxygenclass:: bw64::Bw64Reader
  :members:
.. doxygenclass:: bw64::Bw64Writer
  :members:

Chunks
######

.. doxygenclass:: bw64::Chunk
  :members:

.. doxygenclass:: bw64::FormatInfoChunk
  :members:

.. doxygenclass:: bw64::ExtraData
  :members:

.. doxygenclass:: bw64::DataChunk
  :members:

.. doxygenclass:: bw64::JunkChunk
  :members:

.. doxygenclass:: bw64::AxmlChunk
  :members:

.. doxygenclass:: bw64::AudioId
  :members:

.. doxygenclass:: bw64::ChnaChunk
  :members:

.. doxygenclass:: bw64::DataSize64Chunk
  :members:

.. doxygenclass:: bw64::UnknownChunk
  :members:

Utilities
#########

.. doxygenfunction:: bw64::utils::fourCC
.. doxygenfunction:: bw64::utils::fourCCToStr
