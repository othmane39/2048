link_directories(${projet1_BINARY_DIR})

add_executable(2048-sdl main.c score.c)

target_link_libraries(2048-sdl SDL SDL_ttf grid m file curl)

install(TARGETS 2048-sdl RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)

