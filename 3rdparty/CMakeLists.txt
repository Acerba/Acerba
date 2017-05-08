cmake_minimum_required (VERSION 2.8.11)
PROJECT(3RDPARTY)

SET_PROPERTY(GLOBAL PROPERTY 3RDPARTY_DIR ${3RDPARTY_SOURCE_DIR})

add_subdirectory(SDL2)
add_subdirectory(vorbis)
add_subdirectory(ogg)
add_subdirectory(openal-soft)
add_subdirectory(OALWrapper)
add_subdirectory(gl3w)
add_subdirectory(imgui)
add_subdirectory(TMXParser)


if(NOT ANDROID)
	set_property(TARGET SDL2 SDL2main SDL2-static gl3w imgui PROPERTY FOLDER 3rdParty)
	set_property(TARGET vorbisfile vorbisenc OALWrapper vorbis ogg PROPERTY FOLDER 3rdParty/Audio)
endif()