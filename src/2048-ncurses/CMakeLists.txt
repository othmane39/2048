link_directories(${projet1_BINARY_DIR})

add_executable(2048-ncurses main.c)

target_link_libraries(2048-ncurses ncurses m grid file)

install(TARGETS 2048-ncurses RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)

