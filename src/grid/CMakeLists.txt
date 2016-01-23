link_directories(${projet1_BINARY_DIR})

add_subdirectory(tests)

add_library(grid grid.c)

install(TARGETS grid ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/lib)


