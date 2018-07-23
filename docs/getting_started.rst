.. getting_started:

Getting Started
###############

Requirements and dependencies
-----------------------------

* compiler with C++11 support
* CMake build system (version 3.5 or later)

**Optional**

* Boost.Test (unit tests only, header-only version, version 1.59 or later)

Installation
------------

Just copy the content of the ``include`` directory to your project or add the
repository as a Git submodule to your project and make sure, that the ``bw64``
folder is in your ``PATH``, that the header files can be found by the compiler.

Alternatively clone the Git repository and install the library system wide using
the CMake build system. See the following instructions for \*nix systems.

.. code-block:: console

  git clone git@github.com:irt-open-source/libbw64.git
  cd libbw64
  mkdir build && cd build
  cmake ..
  make
  make install
