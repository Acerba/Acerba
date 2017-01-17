cmake_minimum_required(VERSION 2.8)

project(ACERBA CXX)

add_subdirectory(${ACERBA_SOURCE_DIR}/3rdparty)


file(GLOB ACERBA_SRC ${ACERBA_SOURCE_DIR}/src/*.cpp ${ACERBA_SOURCE_DIR}/include/*)

add_library(acerba STATIC ${ACERBA_SRC})


# -> Add all include directories. (List variable?)
include_directories(${ACERBA_SOURCE_DIR}/include)

# Check library names.
# -> Use List variable for all used libraries?
target_link_libraries(acerba PUBLIC SDL2 SDL2Main)


# Example Program
# -> Add CMake option

file(GLOB EXAMPLE_SRC ${ACERBA_SOURCE_DIR}/example/*)
add_executable(example ${EXAMPLE_SRC})

target_link_libraries(example PUBLIC acerba)
