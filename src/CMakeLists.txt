# --- bw64 ---
add_library(bw64 INTERFACE)
target_include_directories(bw64
  INTERFACE
  # Headers used from source/build location:
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>  # config.h
  # Headers used from installed location:
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

############################################################
# enable C++11 support
############################################################
if ({CMAKE_VERSION} VERSION_LESS "3.8.0")
  # Note: this is not a complete list of c++ features required by libbw64.
  # What we want is C++11 support and this is a simple way to trigger
  # this for CMake < 3.8
  target_compile_features(bw64 INTERFACE cxx_auto_type cxx_nullptr cxx_range_for)
else()
  target_compile_features(bw64 INTERFACE cxx_std_11)
endif()

############################################################
# export package
############################################################
if(BW64_PACKAGE_AND_INSTALL)
  include(CMakePackageConfigHelpers)

  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/config/bw64Config.cmake.in
    ${PROJECT_BINARY_DIR}/bw64Config.cmake
    INSTALL_DESTINATION
    ${INSTALL_CMAKE_DIR}
    PATH_VARS INSTALL_INCLUDE_DIR INSTALL_LIB_DIR INSTALL_CMAKE_DIR
  )

  write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/bw64ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
  )

  ############################################################
  # install bw64 library
  ############################################################
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/bw64
          DESTINATION ${INSTALL_INCLUDE_DIR}
  )
  install(FILES ${PROJECT_BINARY_DIR}/version.hpp
          DESTINATION ${INSTALL_INCLUDE_DIR}/bw64
  )
  install(FILES
    ${PROJECT_BINARY_DIR}/bw64Config.cmake
    ${PROJECT_BINARY_DIR}/bw64ConfigVersion.cmake
    DESTINATION ${INSTALL_CMAKE_DIR}
  )
  install(TARGETS bw64
    EXPORT bw64Targets
    LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
    RUNTIME DESTINATION "${INSTALL_LIB_DIR}"
    ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
    PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/bw64"
  )
  install(EXPORT bw64Targets DESTINATION ${INSTALL_CMAKE_DIR})
endif()
