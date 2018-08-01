# --- unit tests ---
function(add_bw64_test name)
  add_executable(${name} ${name}.cpp $<TARGET_OBJECTS:catch_main>)
  target_link_libraries(${name}
    PRIVATE
    libbw64
  )
  target_include_directories(${name} PRIVATE
    ${CMAKE_SOURCE_DIR}/submodules
  )
  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:${name}>
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data
  )
endfunction()

# this object library provides the main() function and is compiled only once and
# then included into each test
add_library(catch_main OBJECT catch_main.cpp)
target_include_directories(catch_main PUBLIC ${CMAKE_SOURCE_DIR}/submodules)

add_bw64_test(utils_tests)
add_bw64_test(chunk_tests)
add_bw64_test(file_tests)
