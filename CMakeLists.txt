cmake_minimum_required(VERSION 3.5)
project(wowdb)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" "${CMAKE_MODULE_PATH}")

find_package(LevelDB REQUIRED)
find_package(Boost REQUIRED)

include(CMake/duktape.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES object.cpp object.h string.cpp string.h value.cpp value.h store.cpp store.h utils.cpp utils.h undefined.cpp undefined.h number.cpp number.h array.cpp array.h)

add_executable(wowdb main.cpp ${SOURCE_FILES})
target_link_libraries(wowdb duktape leveldb uuid)

add_executable(wowdb_test test.cpp ${SOURCE_FILES})
target_link_libraries(wowdb_test duktape leveldb uuid gtest)