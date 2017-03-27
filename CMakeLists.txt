cmake_minimum_required(VERSION 2.8)

project(ACERBA)
include("cmake/ProjectBuild.cmake" REQUIRED)

BuildBegin()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_subdirectory(${ACERBA_SOURCE_DIR}/3rdparty)

find_package(OpenGL)

file(GLOB SOURCES ${ACERBA_SOURCE_DIR}/src/Ace/*.cpp ${ACERBA_SOURCE_DIR}/include/Ace/*)
set(INCLUDES ${ACERBA_SOURCE_DIR}/include ${ACERBA_SOURCE_DIR}/3rdparty/SDL2/include ${ACERBA_SOURCE_DIR}/3rdparty/Khronos ${ACERBA_SOURCE_DIR}/3rdparty/gl3w/include/
			 ${ACERBA_SOURCE_DIR}/3rdparty/stb/ ${ACERBA_SOURCE_DIR}/3rdparty/OALWrapper/include/ ${ACERBA_SOURCE_DIR}/3rdparty/rapidjson/include 
			${ACERBA_SOURCE_DIR}/3rdparty/nuklear ${ACERBA_SOURCE_DIR}/3rdparty/vorbis/include ${ACERBA_SOURCE_DIR}/3rdparty/ogg/include)

set(LIBS SDL2 SDL2main gl3w ${OPENGL_gl_LIBRARY} OALWrapper)

if(ANDROID)
	set(DEFINES ANDROID GL_GLEXT_PROTOTYPES)
	set(LIBS SDL2 android log openal OALWrapper dl GLESv1_CM GLESv2)

endif()

BuildLibrary(acerba STATIC)


#file(GLOB ACERBA_SRC ${ACERBA_SOURCE_DIR}/src/Ace/*.cpp ${ACERBA_SOURCE_DIR}/include/Ace/*)
#add_library(acerba STATIC ${ACERBA_SRC})

# -> Add all include directories. (List variable?)
#target_include_directories(acerba PUBLIC ${ACERBA_SOURCE_DIR}/include ${ACERBA_SOURCE_DIR}/3rdparty/SDL2/include ${ACERBA_SOURCE_DIR}/3rdparty/Khronos ${ACERBA_SOURCE_DIR}/3rdparty/gl3w/include/ ${ACERBA_SOURCE_DIR}/3rdparty/stb/ 
#${ACERBA_SOURCE_DIR}/3rdparty/OALWrapper/include/)

# Check library names.
# -> Use List variable for all used libraries?
#target_link_libraries(acerba PUBLIC SDL2 SDL2main gl3w ${OPENGL_gl_LIBRARY} OALWrapper)
#target_link_libraries(acerba PUBLIC ${ACERBA_SOURCE_DIR}/libs/libEGL.lib ${ACERBA_SOURCE_DIR}/libs/libGLESV2.lib)


# Example Program
# -> Add CMake option

file(GLOB SOURCES ${ACERBA_SOURCE_DIR}/example/main.cpp)
set(LIBS acerba OALWrapper)

if(ANDROID OR PB_ANDROID)

	list(APPEND SOURCES "${ACERBA_SOURCE_DIR}/3rdparty/SDL2/src/main/android/SDL_android_main.c")
	message(STATUS ${SOURCES})	
	BuildApplication(main) # libmain.so -> SDL2 Activity

else()
	BuildApplication(example)
endif()


#file(GLOB EXAMPLE_SRC ${ACERBA_SOURCE_DIR}/example/*)
#add_executable(example ${EXAMPLE_SRC})

#target_link_libraries(example PUBLIC acerba)
