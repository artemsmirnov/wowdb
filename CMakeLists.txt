cmake_minimum_required(VERSION 3.5)
project(wowdb)

include(duktape.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)

add_executable(wowdb ${SOURCE_FILES})
target_link_libraries(wowdb duktape)
