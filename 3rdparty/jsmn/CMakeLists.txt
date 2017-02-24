cmake_minimum_required(VERSION 2.8)

project(JSMN C)

set(SOURCES ${JSMN_SOURCE_DIR}/jsmn.c)
set(INCLUDES ${JSMN_SOURCE_DIR})

BuildLibrary(jsmn STATIC)
