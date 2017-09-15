cmake_minimum_required(VERSION 2.8)

project(ACERBA)
include("cmake/ProjectBuild.cmake" REQUIRED)

set(ACERBA_MOVELIBS TRUE CACHE BOOL "" )
set(ACERBA_BUILD_EXAMPLE FALSE CACHE BOOL "")

BuildBegin()


set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_subdirectory(${ACERBA_SOURCE_DIR}/3rdparty)

find_package(OpenGL)

file(GLOB SOURCES ${ACERBA_SOURCE_DIR}/src/Ace/*.cpp ${ACERBA_SOURCE_DIR}/include/Ace/*)
set(INCLUDES
    ${ACERBA_SOURCE_DIR}/include
    ${ACERBA_SOURCE_DIR}/3rdparty/SDL2/include
    ${ACERBA_SOURCE_DIR}/3rdparty/Khronos
    ${ACERBA_SOURCE_DIR}/3rdparty/gl3w/include/
    ${ACERBA_SOURCE_DIR}/3rdparty/stb/
    ${ACERBA_SOURCE_DIR}/3rdparty/OALWrapper/include/
    ${ACERBA_SOURCE_DIR}/3rdparty/rapidjson/include
    ${ACERBA_SOURCE_DIR}/3rdparty/vorbis/include
    ${ACERBA_SOURCE_DIR}/3rdparty/ogg/include
    ${ACERBA_SOURCE_DIR}/3rdparty/imgui/include
    ${ACERBA_SOURCE_DIR}/3rdparty/TMXLoader/TMXLoader
)

if (UNIX)
    set(LIBS SDL2 SDL2main gl3w ${OPENGL_gl_LIBRARY} OALWrapper imgui TMXLoader openal)
else ()
    set(LIBS SDL2 SDL2main gl3w ${OPENGL_gl_LIBRARY} OALWrapper imgui TMXLoader ${ACERBA_SOURCE_DIR}/3rdparty/AL/OpenAL32.lib)
endif()

if(ANDROID)
	set(DEFINES ANDROID GL_GLEXT_PROTOTYPES)
	set(LIBS SDL2 android log openal OALWrapper dl GLESv1_CM GLESv2 imgui TMXLoader)
endif()

if(ANDROID)
	BuildLibrary(acerba SHARED)
else()
	BuildLibrary(acerba STATIC)
endif()


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
if(ACERBA_BUILD_EXAMPLE)

	file(GLOB SOURCES ${ACERBA_SOURCE_DIR}/example/main.cpp)
	set(LIBS acerba OALWrapper)

	if(ANDROID OR PB_ANDROID)

		#list(APPEND SOURCES "${ACERBA_SOURCE_DIR}/3rdparty/SDL2/src/main/android/SDL_android_main.c")
		list(APPEND LIBS openal)	
		BuildApplication(main) # libmain.so -> SDL2 Activity

	else()
		BuildApplication(example)
	endif()
endif()

if(PB_MAIN)
	if(PB_ANDROID)
		add_custom_command(TARGET ProjectBuild POST_BUILD
			COMMAND ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=acerba/Application.mk APP_BUILD_SCRIPT=acerba/Android.mk NDK_LIBS_OUT=acerba/libs BASE_PATH="${ACERBA_SOURCE_DIR}"
			WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/android")
			
		add_custom_command(TARGET ProjectBuild POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/android/acerba/libs/" "${PROJECT_BINARY_DIR}/android/libs/")
	endif()
endif()

SET (ACERBA_BUILD_ROOT
   ${CMAKE_BINARY_DIR}
   CACHE PATH
   "Acerba Build Root"
)

# Set DLL directory
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin CACHE PATH "")

# Set Exe directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin CACHE PATH "")

# Set LIB directory
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libs CACHE PATH "")

if(WIN32 AND ACERBA_MOVELIBS)

	set(RENAME_LIBS vorbis.lib vorbisfile.lib)
	
	foreach(LIB ${RENAME_LIBS})
			
		add_custom_command(TARGET acerba PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/${LIB}" "${ACERBA_BUILD_ROOT}/lib${LIB}"
			DEPENDS "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/${LIB}"
			WORKING_DIRECTORY ${ACERBA_BUILD_ROOT}
		)
		
	endforeach() 

	add_custom_command(TARGET acerba PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/SDL2.lib" "${ACERBA_BUILD_ROOT}/SDL2.lib"
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/SDL2.lib" "${ACERBA_BUILD_ROOT}/SDL.lib"
		DEPENDS "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/Debug/SDL2.lib"
		WORKING_DIRECTORY ${ACERBA_BUILD_ROOT}
	)

	add_custom_command(TARGET acerba PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${ACERBA_SOURCE_DIR}/3rdparty/AL/OpenAL32.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/OpenAL32.dll"
		DEPENDS "${ACERBA_SOURCE_DIR}/3rdparty/AL/OpenAL32.dll"
		WORKING_DIRECTORY ${ACERBA_BUILD_ROOT}
	)


endif()
