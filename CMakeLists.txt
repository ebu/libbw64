cmake_minimum_required(VERSION 3.5)

############################################################
# set global paths
############################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include_directories(include)

############################################################
# init project
############################################################
project(libbw64 VERSION 0.10.0)

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(IS_ROOT_PROJECT TRUE)
else()
  set(IS_ROOT_PROJECT FALSE)
endif()

string(TIMESTAMP PROJECT_BUILD_DATE)
set(PROJECT_DESCRIPTION "ITU-R BS.2088 Library")

############################################################
# add new CMAKE_BUILD_TYPE coverage
############################################################
SET(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage")
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage")

############################################################
# user config options
############################################################
include(FeatureSummary)
option(BW64_EXAMPLES "Build examples" ${IS_ROOT_PROJECT})
option(BW64_UNIT_TESTS "Build units tests" ${IS_ROOT_PROJECT})
option(BW64_PACKAGE_AND_INSTALL "Package and install libbw64" ${IS_ROOT_PROJECT})
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR CMake CACHE PATH "Installation directory for CMake files")
else()
  set(INSTALL_CMAKE_DIR share/cmake/bw64 CACHE PATH "Installation directory for CMake files")
endif()

############################################################
# configure files
############################################################
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.hpp")

############################################################
# add targets
############################################################
add_subdirectory(src)

if(BW64_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BW64_UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

############################################################
# FeatureSummary
############################################################
add_feature_info(BW64_EXAMPLES ${BW64_EXAMPLES} "Build examples")
add_feature_info(BW64_UNIT_TESTS ${BW64_UNIT_TESTS} "Build units tests")
add_feature_info(BW64_PACKAGE_AND_INSTALL ${BW64_PACKAGE_AND_INSTALL} "Package and install libbw64")
feature_summary(WHAT ALL)

#########################################################
# very minimal cpack configuration
#########################################################
if(BW64_PACKAGE_AND_INSTALL)
  include(InstallRequiredSystemLibraries)
  set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
  set (CPACK_GENERATOR "TGZ;ZIP")
  include (CPack)

  #########################################################
  # custom uninstall target
  #########################################################
  configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
      IMMEDIATE @ONLY
  )
  add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
endif()
