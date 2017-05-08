cmake_minimum_required(VERSION 2.8)

project(TMXPARSER)

set(INCLUDES ${TMXPARSER_SOURCE_DIR}/include)
file(GLOB SOURCES ${TMXPARSER_SOURCE_DIR}/src/*.cpp)

BuildLibrary(tmxparser STATIC)