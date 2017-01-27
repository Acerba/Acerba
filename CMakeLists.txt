cmake_minimum_required(VERSION 2.8)

project(ACERBA CXX)

add_subdirectory(${ACERBA_SOURCE_DIR}/3rdparty)

find_package(OpenGL)



file(GLOB ACERBA_SRC ${ACERBA_SOURCE_DIR}/src/Ace/*.cpp ${ACERBA_SOURCE_DIR}/include/Ace/*)

add_library(acerba STATIC ${ACERBA_SRC})


# -> Add all include directories. (List variable?)
include_directories(${ACERBA_SOURCE_DIR}/include ${ACERBA_SOURCE_DIR}/3rdparty/SDL2/include ${ACERBA_SOURCE_DIR}/3rdparty/Khronos ${ACERBA_SOURCE_DIR}/3rdparty/gl3w/include ${ACERBA_SOURCE_DIR}/3rdparty/stb/)

# Check library names.
# -> Use List variable for all used libraries?
target_link_libraries(acerba PUBLIC SDL2 SDL2main gl3w ${OPENGL_gl_LIBRARY})
target_link_libraries(acerba PUBLIC ${ACERBA_SOURCE_DIR}/libs/libEGL.lib ${ACERBA_SOURCE_DIR}/libs/libGLESV2.lib)


# Example Program
# -> Add CMake option

file(GLOB EXAMPLE_SRC ${ACERBA_SOURCE_DIR}/example/*)
add_executable(example ${EXAMPLE_SRC})

target_link_libraries(example PUBLIC acerba)
