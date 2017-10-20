cmake_minimum_required(VERSION 3.0)

project(MATVEK)

set(MATVEK_CPP17 FALSE CACHE BOOL "")
set(MATVEK_DEBUG FALSE CACHE BOOL "")
set(MATVEK_EXPERIMENTAL FALSE CACHE BOOL "")

if (MATVEK_CPP17)
    add_definitions(-DMV_CONSTEXPR 1)
endif()

if (MATVEK_DEBUG)
    add_definitions(-DMV_DEBUG 1)
endif()

if (MATVEK_EXPERIMENTAL)
    add_definitions(-DMV_EXPERIMENTAL 1)
endif()

file(GLOB mvlibfiles
    ${MATVEK_SOURCE_DIR}/*.hpp
    ${MATVEK_SOURCE_DIR}/classes/*.hpp
    ${MATVEK_SOURCE_DIR}/functions/*.hpp
    ${MATVEK_SOURCE_DIR}/utilities/*.hpp
)

add_library(matveklib INTERFACE)

target_compile_options(matveklib INTERFACE )

if(MSVC)
    target_include_directories(matveklib INTERFACE mvlibfiles /Wall)
else()
    target_include_directories(matveklib INTERFACE mvlibfiles -W -Wall)
endif()
