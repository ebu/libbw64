.. libbw64 documentation master file

.. image:: irt_logo.png
   :alt: IRT Logo

-------------------------------

libbw64 – ITU-R BS.2088 Library
===============================

.. toctree::
  :hidden:
  :maxdepth: 1

  getting_started
  tutorial
  changelog

.. toctree::
  :hidden:
  :maxdepth: 2
  :caption: Reference

  reference

The `libbw64 <https://github.com/irt-open-source/libbw64>`_ library is a
lightweight C++ header only library to read and write BW64 files. BW64 is
standardised as `Recommendation ITU-R BS.2088
<https://www.itu.int/rec/R-REC-BS.2088/en>`_ and the successor of RF64. So it
already contains necessary extensions to support files which are bigger than 4
GB. Apart from that an BW64 file is able to contain the ADM metadata and link it
with the audio tracks in the file. To do that a BW64 specifies two new RIFF
chunks – the ``axml`` chunk and the ``chna`` chunk. To parse, create, modify and
write the ADM metadata in the ``axml`` chunk you may use the `libadm
<https://github.com/irt-open-source/libadm>`_ library.

Features
--------

- no dependencies (only Boost 1.59 or later for the unit tests)
- support file sizes bigger than 4 GB (``ds64`` chunk)
- read and write ``axml`` and ``chna`` chunks
- 16, 24, and 32 bit integer file formats

Acknowledgement
---------------

This project has received funding from the European Union’s Horizon 2020
research and innovation programme under grant agreement No 687645.
