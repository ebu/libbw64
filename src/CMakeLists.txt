# --- libbw64 ---
add_library(libbw64 INTERFACE)
target_include_directories(libbw64
  INTERFACE
  # Headers used from source/build location:
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>  # config.h
  # Headers used from installed location:
    $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
)

############################################################
# export package
############################################################
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/config/libbw64Config.cmake.in
  ${CMAKE_BINARY_DIR}/libbw64Config.cmake
  INSTALL_DESTINATION
  ${INSTALL_CMAKE_DIR}
  PATH_VARS INSTALL_INCLUDE_DIR INSTALL_LIB_DIR INSTALL_CMAKE_DIR
)

write_basic_package_version_file(
  ${CMAKE_BINARY_DIR}/libbw64ConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
)

############################################################
# install libbw64 library
############################################################
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/bw64
        DESTINATION ${INSTALL_INCLUDE_DIR}
)
install(FILES ${PROJECT_BINARY_DIR}/version.hpp
        DESTINATION ${INSTALL_INCLUDE_DIR}/bw64
)
install(FILES
  ${CMAKE_BINARY_DIR}/libbw64Config.cmake
  ${CMAKE_BINARY_DIR}/libbw64ConfigVersion.cmake
  DESTINATION ${INSTALL_CMAKE_DIR}
)
install(TARGETS libbw64
  EXPORT libbw64Targets
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  RUNTIME DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/bw64"
)
install(EXPORT libbw64Targets DESTINATION ${INSTALL_CMAKE_DIR})
