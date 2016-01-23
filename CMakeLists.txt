cmake_minimum_required(VERSION 2.6)

project(projet-2048)

add_definitions(-std=c99 -Wall -Werror)

include_directories(include/)

add_subdirectory(src)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

enable_testing()

add_test(test-grid-1-create-delete src/grid/tests/test-grid1)
add_test(test-grid-2-set-get-tile src/grid/tests/test-grid2)
add_test(test-grid-4-can-move src/grid/tests/test-grid4)
add_test(test-grid-4-game-over src/grid/tests/test-grid4)