cmake_minimum_required(VERSION 3.5)

############################################################
# set global paths
############################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include_directories(include)

############################################################
# init project
############################################################
project(libbw64 VERSION 0.9.0)
string(TIMESTAMP PROJECT_BUILD_DATE)
set(PROJECT_DESCRIPTION "ITU-R BS.2088 Library")

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

############################################################
# user config options
############################################################
include(FeatureSummary)
option(EXAMPLES "Build example programmes" TRUE)
option(UNIT_TESTS "Build units tests" TRUE)
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR CMake CACHE PATH "Installation directory for CMake files")
else()
  set(INSTALL_CMAKE_DIR share/cmake/libbw64 CACHE PATH "Installation directory for CMake files")
endif()

############################################################
# enable C++11 support
############################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

############################################################
# find libraries
############################################################
find_package(Boost)

# --- Unit tests ---
if(UNIT_TESTS AND NOT Boost_FOUND)
  message(STATUS "Boost not found: Disabling UNIT_TESTS")
  set(UNIT_TESTS FALSE)
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

if(EXAMPLES)
  add_subdirectory(examples)
endif()

if(UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

############################################################
# FeatureSummary
############################################################
add_feature_info(EXAMPLES ${EXAMPLES} "Build example programmes")
add_feature_info(UNIT_TESTS ${UNIT_TESTS} "Build unit tests")
feature_summary(WHAT ALL)

#########################################################
# very minimal cpack configuration
#########################################################
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
