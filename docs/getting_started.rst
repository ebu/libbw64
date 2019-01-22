.. getting_started:

Getting Started
###############

Requirements and dependencies
-----------------------------

* compiler with C++11 support
* CMake build system (version 3.5 or later)

Installation
------------

macOS
*****

On macOS you can also use homebrew to install the library. You just have to
add the IRT's NGA homebrew tap and can then use the usual install command.

.. code:: console

    brew tap irt-open-source/homebrew-nga
    brew install libbw64

Manual
******

Alternatively clone the Git repository and install the library system wide using
the CMake build system. See the following instructions for \*nix systems.

.. code-block:: console

  git clone git@github.com:irt-open-source/libbw64.git
  cd libbw64
  mkdir build && cd build
  cmake ..
  make
  make install

Copy headers
************

The ``libbw64`` is a header-only library so installing the library is not by all
means necessary. It is also possible to just copy the content of the ``include``
directory to your project and make sure, that the ``bw64`` folder is in your
``PATH``, that the header files can be found by the compiler.

CMake
-----

As the library uses CMake as a build system it is really easy to set up and
use if your project does too. Assuming you have installed the library, the
following code shows a complete CMake example to compile a program which
uses the ``libbw64``.

.. code:: console

  cmake_minimum_required(VERSION 3.8)
  project(libbw64_example VERSION 1.0.0 LANGUAGES CXX)

  find_package(bw64 REQUIRED)

  add_executable(example example.cpp)
  target_link_libraries(example PRIVATE bw64)

If you prefer not to install the library on your system you can also use the
library as a subproject. You can just add the library as a CMake subproject.
Just add the folder containing the repository to your project and you can use
the ``bw64`` target.

.. code:: console

  cmake_minimum_required(VERSION 3.5)
  project(libbw64_example VERSION 1.0.0 LANGUAGES CXX)

  add_subdirectory(submodules/libbw64)

  add_executable(example example.cpp)
  target_link_libraries(example PRIVATE bw64)

.. note::
    If ``libbw64`` is used as a CMake subproject the default values of the options

      * ``BW64_UNIT_TESTS``
      * ``BW64_EXAMPLES``
      * ``BW64_PACKAGE_AND_INSTALL``

    are automatically set to ``FALSE``.
