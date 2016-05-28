cmake_minimum_required(VERSION 3.5)
project(wowdb)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" "${CMAKE_MODULE_PATH}")

find_package(LevelDB REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost REQUIRED COMPONENTS system thread regex )

include(CMake/duktape.cmake)

set(SOURCE_FILES object.cpp object.h string.cpp string.h value.cpp value.h store.cpp store.h utils.cpp utils.h undefined.cpp undefined.h number.cpp number.h array.cpp array.h)

add_executable(wowdb main.cpp ${SOURCE_FILES})
set_property(TARGET wowdb PROPERTY CXX_STANDART 14)
target_link_libraries(wowdb duktape leveldb uuid ${Boost_LIBRARIES} pthread)

add_executable(wowdb_test test.cpp ${SOURCE_FILES})
target_link_libraries(wowdb_test duktape leveldb uuid gtest ${Boost_LIBRARIES})