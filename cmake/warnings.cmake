#warnings.cmake
#-------------------
#
#.. macro:: compiler_warning_flags(<prefix>)
#
#    :param <prefix>:
#        prefix for variable names to store flags
#
# This function returns a list of compiler flags
# that can be used as compiler definitions to enable all compiler warnings
# and change the behaviour related to warnings in general
# Use it with target_compile_definitions or similar methods.
#
# Will set the following variables:
#   - <prefix>_WARN_ALL   flags to enable "all" warnings
#   - <prefix>_WARN_ERROR flags to treat warnings as errors
#
# Note: "all" does not promise to actually be _all_.
# It's more like "very very high" :) (i.e. cross-commpiler "-W -Wall")
#
macro(compiler_warning_flags PREFIX)
  # might be "Clang" or "AppleClang"
  # see https://cmake.org/cmake/help/v3.0/policy/CMP0025.html#policy:CMP0025
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(${PREFIX}_WARN_ALL "-W -Wall")
    set(${PREFIX}_WARN_ERROR "-Werror")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(${PREFIX}_WARN_ALL "-W -Wall")
    set(${PREFIX}_WARN_ERROR "-Werror")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++, todo
    set(${PREFIX}_WARN_ALL)
    set(${PREFIX}_WARN_ERROR)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(${PREFIX}_WARN_ALL "/W3")
    set(${PREFIX}_WARN_ERROR "/WX")
  endif()
endmacro()
