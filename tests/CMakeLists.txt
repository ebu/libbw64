include(${PROJECT_SOURCE_DIR}/submodules/catch2.cmake)

# --- unit tests ---
function(add_bw64_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name}
    PRIVATE
    bw64
    catch2
  )
  target_include_directories(${name} PRIVATE
    ${CMAKE_SOURCE_DIR}/submodules
  )
  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:${name}>
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/test_data"
  )
endfunction()

# copy test files so unit test can find them relative to their running location
# when executed as "test" target
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/test_data" DESTINATION ${CMAKE_BINARY_DIR})

add_bw64_test(utils_tests)
add_bw64_test(chunk_tests)
add_bw64_test(file_tests)
