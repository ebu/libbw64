# --- tools ---
add_executable(bw64_info bw64_info.cpp)
target_link_libraries(bw64_info libbw64)

add_executable(bw64_extract_adm bw64_extract_adm.cpp)
target_link_libraries(bw64_extract_adm libbw64)

add_executable(bw64_create_sine bw64_create_sine.cpp)
target_link_libraries(bw64_create_sine libbw64)

add_executable(bw64_read_write bw64_read_write.cpp)
target_link_libraries(bw64_read_write libbw64)
