cmake_minimum_required(VERSION 2.8)

project (imgui)

include_directories(include)

file(GLOB SRC src/*.cpp)

add_library(imgui STATIC ${SRC})