link_directories(${projet1_BINARY_DIR})

add_executable(test-grid1 test-grid1.c)
add_executable(test-grid2 test-grid2.c)
add_executable(test-grid3 test-grid3.c)
add_executable(test-grid4 test-grid4.c)
add_executable(test-grid5 test-grid5.c)

target_link_libraries(test-grid1 grid m file)
target_link_libraries(test-grid2 grid m file)
target_link_libraries(test-grid3 grid m file)
target_link_libraries(test-grid4 grid m file)
target_link_libraries(test-grid5 grid m file)

