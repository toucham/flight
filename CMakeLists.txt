cmake_minimum_required(VERSION 3.15)

project(CppWebServer)

add_subdirectory(include)
add_subdirectory(src)

option(BUILD_EXAMPLES "Whether to build example or not" ON)

if(BUILD_EXAMPLES)
    message("Building examples...")
    add_subdirectory(example)
endif()