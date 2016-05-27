cmake_minimum_required(VERSION 3.5)
project(wowdb)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" "${CMAKE_MODULE_PATH}")

find_package(LevelDB REQUIRED)

include(CMake/duktape.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp object.cpp object.h string.cpp string.h value.cpp value.h store.cpp store.h utils.cpp utils.h undefined.cpp undefined.h number.cpp number.h)

add_executable(wowdb ${SOURCE_FILES})
target_link_libraries(wowdb duktape leveldb uuid)
