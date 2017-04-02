cmake_minimum_required(VERSION 2.8)

project (imgui)

include_directories(include)

file(GLOB SOURCES src/*.cpp)

BuildLibrary(imgui STATIC)
