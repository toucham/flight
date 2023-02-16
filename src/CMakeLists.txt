project(CppWebServerSource)

add_library(main main.cpp)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)