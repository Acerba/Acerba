# Copyright (c) 2017 Atte Vuorinen <attevuorinen@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgement in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.


# Variables:
# INCLUDES
# SOURCES
# LIBS
# DEFINES
# DEFINES_PRIVATE
# ARGS

# BuildCallback() # WIP


##########
# Values #
##########

#TODO: Generic system
#TODO: Platforms. (Generic, Android, etc.)

set(PB_USE_ANDROID_MK FALSE)
#set(PB TRUE CACHE INTERNAL "")


if(ANDROID)
	set(PB_ANDROID TRUE CACHE BOOL "" )
else()
	set(PB_ANDROID FALSE CACHE BOOL "" )
endif()

if(PB_RECURSION)
	set(PB_RECURSION TRUE CACHE BOOL "Sub Project" )
else()
	set(PB_MAIN TRUE CACHE BOOL "Main Project" )
endif()


##########
# Config #
##########

set(PB_RELEASE FALSE CACHE BOOL "Release")
set(PB_FLAGS CACHE STRING "General flags")
set(PB_CXX_FLAGS CACHE STRING "C++ Compiler Flags")
set(PB_C_FLAGS CACHE STRING "C Compiler Flags")

set(PB_FORCE_PROJECT FALSE CACHE BOOL "Use root project ()" )

if(PB_ANDROID OR ANDROID)

set(PB_ANDROID_ABI "armeabi" "armeabi-v7a" "x86" CACHE STRING "Android ABI Targets")
set(PB_ANDROID_API "9" CACHE STRING "Android API LEVEL")

set(ANDROID_STL_VALUES
	"none;system;system_re;gabi++_static;gabi++_shared;stlport_static;stlport_shared;gnustl_static;gnustl_shared;c++_static;c++_shared;"
)

set(PB_ANDROID_STL "c++_static" CACHE STRING "Android STL")
set_property(CACHE PB_ANDROID_STL PROPERTY STRINGS ${ANDROID_STL_VALUES})

set(ANDROID_BUILD_SYSTEMS
	"ant debug install;gradlew installDebug;"
)

set(PB_ANDROID_BUILD_SYSTEM "gradlew installDebug" CACHE STRING "Android Build System")
set_property(CACHE PB_ANDROID_BUILD_SYSTEM PROPERTY STRINGS ${ANDROID_BUILD_SYSTEMS})

set(PB_ANDROID FALSE CACHE BOOL "Is Android Project" )
set(PB_ANDROID_TEMPLATE "${PROJECT_SOURCE_DIR}/android_template" CACHE PATH "Android Project Template")
set(PB_ANDROID_NDK TRUE CACHE BOOL "Call ndk-build when building" )

endif()

set(PB_TOOLCHAIN "${CMAKE_TOOLCHAIN_FILE}" CACHE FILEPATH "SubProject toolchain" )
set(PB_TARGET_GENERATOR "Unix Makefiles" CACHE STRING "SubProject generator")

##########
# Macros #
##########

# BuildNative
# Build using native build pipeline
# For Android this means build using NDK,
# This can be also called using BuildNDK for Android only.
macro(BuildNative)

	if(PB_ANDROID OR ANDROID)
		BuildNDK()
	endif()

endmacro()

# BuildBegin
# Setups Build environment
# Only call this on ROOT CMakeLists.txt file.
macro(BuildBegin)

	# TODO Add Root check. (just in case)
	
		if(PB_RECURSION)

			# Back to "root"
			#get_filename_component(PARENT_DIR ${PROJECT_BINARY_DIR} DIRECTORY)
			#get_filename_component(BUILD_DIR ${PARENT_DIR} DIRECTORY)
			message(STATUS "Build Dir: ${BUILD_DIR}")
	
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/obj/${ANDROID_ABI}/lib)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/obj/${ANDROID_ABI}/lib)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/obj/${ANDROID_ABI}/bin)


			# Strips Debug information from the release binaries.
			if(PB_RELEASE)
				set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
				set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
			endif()

			# Set Recursion flags
			set(CMAKE_C_FLAGS "${PB_C_FLAGS}")
			set(CMAKE_CXX_FLAGS "${PB_CXX_FLAGS}")

			set(CMAKE_C_FLAGS_RELEASE "${PB_C_FLAGS}")
			set(CMAKE_CXX_FLAGS_RELEASE "${PB_CXX_FLAGS}")

			set(CMAKE_C_FLAGS_DEBUG "${PB_C_FLAGS}")
			set(CMAKE_CXX_FLAGS_DEBUG "${PB_CXX_FLAGS}")

		elseif(PB_MAIN)

			if(NOT TARGET PSetup)
				set(PB_INITIALIZED FALSE)
			endif()
		
			if(NOT PB_INITIALIZED)
				set(PB_INITIALIZED TRUE CACHE INTERNAL "")
								
				set(BUILD_DIR ${PROJECT_BINARY_DIR})
			
				add_custom_target(PSetup)
				add_custom_target(PBuild)
				add_custom_target(PBuildPost)
				add_custom_target(PRun)
				
				add_custom_target(PSetup_${PROJECT_NAME} DEPENDS PSetup)
				add_custom_target(PBuild_${PROJECT_NAME} DEPENDS PBuild)
				add_custom_target(PBuildPost_${PROJECT_NAME} DEPENDS PBuild)

				if(PB_CXX_FLAGS)
					list(APPEND PB_FLAGS "-DPB_CXX_FLAGS=${PB_CXX_FLAGS}")
				endif()

				if(PB_C_FLAGS)
					list(APPEND PB_FLAGS "-DPB_C_FLAGS=${PB_C_FLAGS}")
				endif()

				if(PB_ANDROID OR ANDROID)
					BuildAndroid()
				endif()

				#add_dependencies(PBuildPost PBuild)
				add_dependencies(PRun PBuild)

				add_library(PBuildHook SHARED)
				message(STATUS "PB INFO: Don't bother about above warning about PBuildHook, that is intended dirty hack for Make dependencies.")
				set_target_properties(PBuildHook PROPERTIES LINKER_LANGUAGE CXX)
				

				add_dependencies(PBuildHook PBuild)

				set_property(TARGET PSetup PBuild PRun PBuildHook PBuildPost PROPERTY FOLDER ProjectBuild)
				set_property(TARGET PSetup_${PROJECT_NAME} PBuild_${PROJECT_NAME} PBuildPost_${PROJECT_NAME} PROPERTY FOLDER ProjectBuild/Hooks)
							
				add_custom_command(TARGET PBuild POST_BUILD COMMAND cmake --build . --target PBuildPost WORKING_DIRECTORY "${BUILD_DIR}")
			else()
					
				add_custom_target(PSetup_${PROJECT_NAME})
				add_custom_target(PBuild_${PROJECT_NAME})
				add_custom_target(PBuildPost_${PROJECT_NAME})
				
				set_property(TARGET PSetup_${PROJECT_NAME} PBuild_${PROJECT_NAME} PBuildPost_${PROJECT_NAME} PROPERTY FOLDER ProjectBuild/Hooks)

				add_dependencies(PSetup PSetup_${PROJECT_NAME})
				add_dependencies(PBuild PBuild_${PROJECT_NAME})
				add_dependencies(PBuildPost PBuildPost_${PROJECT_NAME})
									
			endif()

		endif()
		
		set(PB_BUILD_DIR ${BUILD_DIR} CACHE INTERNAL "")


endmacro(BuildBegin)

# BuildIgnore
macro(BuildIgnore)
	return()
endmacro(BuildIgnore)

# Build Library
macro(BuildLibrary name type)

	BuildNDK()

	if((ANDROID AND PB_RECURSION) OR NOT ANDROID OR PB_FORCE_PROJECT)

		BuildTargetBegin(${name})

		if(${type} STREQUAL SHARED)
			add_library(${name} SHARED ${SOURCES})
		else()
			add_library(${name} STATIC ${SOURCES})
		endif()

		foreach(LIB ${LIBS})
			if(EXISTS "${BUILD_DIR}/android/libs/${ANDROID_ABI}/lib${LIB}.so")
				target_link_libraries(${name} PUBLIC "${BUILD_DIR}/android/libs/${ANDROID_ABI}/lib${LIB}.so")
			endif()
		endforeach()

		BuildTargetEnd(${name})

	endif()

endmacro(BuildLibrary name)

# BuildApplication
macro(BuildApplication name)

if(ANDROID AND NOT PB_FORCE_PROJECT)

	BuildLibrary(${name} SHARED ${SOURCES})

else()

	BuildTargetBegin(${name})
	add_executable(${name} ${ARGS} ${SOURCES})
	BuildTargetEnd(${name})

endif()

endmacro(BuildApplication name)


###########
# Helpers #
###########

# BuildTargetBegin
macro(BuildTargetBegin name)

	include_directories(${INCLUDES})

endmacro(BuildTargetBegin name)

# BuildTargetEnd
macro(BuildTargetEnd name)

	target_link_libraries(${name} PUBLIC ${LIBS})
	target_compile_definitions(${name} PUBLIC ${DEFINES})
	target_compile_definitions(${name} PRIVATE ${DEFINES_PRIVATE})

endmacro(BuildTargetEnd name)

#############
# Platforms #
#############

# BuildNDK
macro(BuildNDK)

	if(PB_ANDROID OR ANDROID)

		if(EXISTS "${PROJECT_SOURCE_DIR}/Android.mk")
			if(NOT PB_RECURSION)

				message(STATUS "PB NDK: " ${PROJECT_SOURCE_DIR})

				if(NOT EXISTS "${BUILD_DIR}/jni/${PROJECT_SOURCE_DIR}")
					file(COPY ${PROJECT_SOURCE_DIR} DESTINATION ${BUILD_DIR}/android/jni)
				endif()

			endif()

			if(NOT PB_FORCE_PROJECT OR PB_RECURSION)
				return()
			endif()

		endif()
	endif()

endmacro(BuildNDK)

# BuildAndroid
macro(BuildAndroid)

	if(PB_ANDROID_NDK)
		add_custom_target(PBuildNDK)
	endif()

	if(EXISTS "${PB_ANDROID_TEMPLATE}/AndroidManifest.xml" AND NOT EXISTS "${BUILD_DIR}/android/AndroidManifest.xml")

		file(REMOVE_RECURSE "${BUILD_DIR}/android")
		get_filename_component(TEMPLATE_NAME ${PB_ANDROID_TEMPLATE} NAME)
		file(COPY ${PB_ANDROID_TEMPLATE} DESTINATION ${BUILD_DIR})
		file(RENAME "${BUILD_DIR}/${TEMPLATE_NAME}" "${BUILD_DIR}/android")

	endif()

	add_custom_command(TARGET PSetup PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "ProjectBuild: Initializing projects..."
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/android/jni/"
		COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/android/libs"
	)

	# Option for this.
	#add_custom_command(TARGET PSetup PRE_BUILD
	#	COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/obj")


	if(PB_ANDROID_NDK)
		add_custom_command(TARGET PBuildNDK PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "ProjectBuild: Starting NDK building... ${PROJECT_BINARY_DIR}"
			COMMAND ndk-build
			WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/android")
	endif()

	foreach(ABI ${PB_ANDROID_ABI})

		add_custom_command(TARGET PSetup PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/obj/${ABI}/")

		if(PB_DEBUG)

			# Temp.
			# TODO 'Generic' Project generation

			set(PB_TARGET_GENERATOR ${CMAKE_GENERATOR})
			add_custom_command(TARGET PSetup PRE_BUILD
				COMMAND cmake -G "${PB_TARGET_GENERATOR}" "${PROJECT_SOURCE_DIR}/" -DBUILD_DIR=${BUILD_DIR} -DPB=TRUE -DANDROID_STL=${PB_ANDROID_STL} -DANDROID_NATIVE_API_LEVEL=${PB_ANDROID_API} -DPB_RECURSION=TRUE -DANDROID_ABI=${ABI} -DOUTPUT_PATH=${PROJECT_BINARY_DIR}/libs/${ABI} -DCMAKE_BUILD_TYPE=Debug -DPB_RELEASE=${PB_RELEASE} ${PB_FLAGS}
				WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")
		else()

			add_custom_command(TARGET PSetup PRE_BUILD
				COMMAND cmake -G "${PB_TARGET_GENERATOR}" "${PROJECT_SOURCE_DIR}/" -DBUILD_DIR=${BUILD_DIR} -DPB=TRUE -DCMAKE_TOOLCHAIN_FILE=${PB_TOOLCHAIN} -DPB_RECURSION=TRUE -DANDROID_ABI=${ABI} -DCMAKE_BUILD_TYPE=Release -DPB_RELEASE=${PB_RELEASE} -DANDROID_STL=${PB_ANDROID_STL} -DANDROID_NATIVE_API_LEVEL=${PB_ANDROID_API} ${PB_FLAGS}
				WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")

		endif()

		add_custom_command(TARGET PBuild PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "ProjectBuild: Updating projects..."
			COMMAND cmake "."
			WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")

		add_custom_command(TARGET PBuild PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "ProjectBuild: Starting building..."
			COMMAND cmake --build "."
			WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")

		add_custom_command(TARGET PBuild PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/obj/${ABI}/lib" "${PROJECT_BINARY_DIR}/android/libs/${ABI}")

	endforeach()

	# Option for this. (PB_Install)
	SEPARATE_ARGUMENTS(PB_ANDROID_BUILD_SYSTEM)
	add_custom_command(TARGET PRun POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "ProjectBuild: Starting project..."
		COMMAND ${PB_ANDROID_BUILD_SYSTEM}
		WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/android")

	# Support for default target.
	if(PB_ANDROID_NDK)
		add_dependencies(PBuildNDK PSetup)
		add_dependencies(PBuild PBuildNDK)
		set_property(TARGET PBuildNDK PROPERTY FOLDER ProjectBuild)
	else()
		add_dependencies(PBuild PSetup)
	endif()

endmacro()
