PROJECT(3RDPARTY)

SET_PROPERTY(GLOBAL PROPERTY 3RDPARTY_DIR ${3RDPARTY_SOURCE_DIR})

add_subdirectory(SDL2)
add_subdirectory(vorbis)
add_subdirectory(ogg)
add_subdirectory(OALWrapper)
add_subdirectory(gl3w)