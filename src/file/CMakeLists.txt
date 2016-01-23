link_directories(${projet1_BINARY_DIR})

add_library(file file.c)

install(TARGETS file ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/lib)

#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
