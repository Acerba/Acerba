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

	set(PB_USE_ANDROID_MK FALSE)
	set(PB_ANDROID_ABI "armeabi" "armeabi-v7a" "x86")


	if(ANDROID)
		set(PB_ANDROID TRUE CACHE BOOL "" )
	else()
		set(PB_ANDROID FALSE CACHE BOOL "" )
	endif()
	
	if(PB_RECURSION)
		set(PB_RECURSION TRUE CACHE BOOL "" )
	else()
		set(PB_MAIN TRUE CACHE BOOL "" )
	endif()
	
	# TODO: More build options.
	set(PB_RELEASE TRUE CACHE BOOL "")
	
	set(PB_FORCE_PROJECT FALSE CACHE BOOL "" )
	set(PB_ANDROID_TEMPLATE "${PROJECT_SOURCE_DIR}/android_template" CACHE PATH "")
	set(PB_ANDROID_NDK TRUE CACHE BOOL "" )
	set(PB_TOOLCHAIN "${CMAKE_TOOLCHAIN_FILE}" CACHE FILEPATH "" )
	set(PB_TARGET_GENERATOR "Unix Makefiles" CACHE STRING "")
	
	macro(BuildBegin)


		if(PB_RECURSION)
		
			# Back to "root"
			get_filename_component(PARENT_DIR ${PROJECT_BINARY_DIR} DIRECTORY)
			get_filename_component(BUILD_DIR ${PARENT_DIR} DIRECTORY)
			message(STATUS ${BUILD_DIR})
		
			set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
			
			
			if(PB_RELEASE)
				set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
				set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
			endif()
			
		
		elseif(PB_ANDROID OR ANDROID)
		
			set(BUILD_DIR ${PROJECT_BINARY_DIR})

			add_custom_target(ProjectSetup)
			add_custom_target(ProjectBuild)
			add_custom_target(ProjectRun)
			
			if(PB_ANDROID_NDK)

				add_custom_target(ProjectBuildNDK)
			
			endif()
			
			set_property(TARGET ProjectSetup ProjectBuild ProjectRun PROPERTY FOLDER ProjectBuild)
			
			if(EXISTS "${PB_ANDROID_TEMPLATE}/build.xml" AND NOT EXISTS "${BUILD_DIR}/android/build.xml")
				
				file(REMOVE_RECURSE "${BUILD_DIR}/android")
				get_filename_component(TEMPLATE_NAME ${PB_ANDROID_TEMPLATE} NAME)
				file(COPY ${PB_ANDROID_TEMPLATE} DESTINATION ${BUILD_DIR})
				file(RENAME "${BUILD_DIR}/${TEMPLATE_NAME}" "${BUILD_DIR}/android")

			
			endif()
			
			add_custom_command(TARGET ProjectSetup PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/android/jni/")
			
			# Option for this.
			#add_custom_command(TARGET ProjectSetup PRE_BUILD
			#	COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/obj")			
				
			add_custom_command(TARGET ProjectSetup PRE_BUILD
				COMMAND ${CMAKE_COMMAND} -E remove_directory "${PROJECT_BINARY_DIR}/android/libs")
				
			if(PB_ANDROID_NDK)
				add_custom_command(TARGET ProjectBuildNDK PRE_BUILD
					COMMAND ndk-build
					WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/android")
			endif()
							
			foreach(ABI ${PB_ANDROID_ABI})
			
				add_custom_command(TARGET ProjectSetup PRE_BUILD
					COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/obj/${ABI}/")
					
				if(PB_DEBUG)
				
					# Temp.
				
					set(PB_TARGET_GENERATOR ${CMAKE_GENERATOR})
					add_custom_command(TARGET ProjectSetup PRE_BUILD
						COMMAND cmake -G "${PB_TARGET_GENERATOR}" "${PROJECT_SOURCE_DIR}/" -DPB_RECURSION=TRUE -DANDROID_ABI=${ABI} -DOUTPUT_PATH=${PROJECT_BINARY_DIR}/libs/${ABI} -DCMAKE_BUILD_TYPE=Debug -DPB_RELEASE=${PB_RELEASE}
						WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")
				else()

					add_custom_command(TARGET ProjectSetup PRE_BUILD
						COMMAND cmake -G "${PB_TARGET_GENERATOR}" "${PROJECT_SOURCE_DIR}/" -DCMAKE_TOOLCHAIN_FILE=${PB_TOOLCHAIN} -DPB_RECURSION=TRUE -DANDROID_ABI=${ABI} -DCMAKE_BUILD_TYPE=Release -DPB_RELEASE=${PB_RELEASE}
						WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")			
					
				endif()
				
				add_custom_command(TARGET ProjectBuild PRE_BUILD
					COMMAND cmake "."
					WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")
								
				add_custom_command(TARGET ProjectBuild PRE_BUILD
					COMMAND cmake --build "."
					WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/obj/${ABI}/")
					
				add_custom_command(TARGET ProjectBuild PRE_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_BINARY_DIR}/obj/${ABI}/lib" "${PROJECT_BINARY_DIR}/android/libs/${ABI}")
									
			endforeach()
			
			
			# Option for this. (PB_Install)
			add_custom_command(TARGET ProjectRun POST_BUILD
				COMMAND ant debug install
				WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/android")
					
					
			# Support for default target.
			
			
			if(PB_ANDROID_NDK)
				add_dependencies(ProjectBuildNDK ProjectSetup)
				add_dependencies(ProjectBuild ProjectBuildNDK)
			else()
				add_dependencies(ProjectBuild ProjectSetup)
			
			endif()
			
			add_dependencies(ProjectRun ProjectBuild)
			
			add_library(ProjectBuildHook SHARED)
			message(STATUS "PB INFO: Don't bother about above warning about ProjectBuildHook, that is intended dirty hack for Make.")
			set_target_properties(ProjectBuildHook PROPERTIES LINKER_LANGUAGE CXX)
			add_dependencies(ProjectBuildHook ProjectBuild)

		endif()

	endmacro(BuildBegin)

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

	macro(BuildIgnore)

		return()

	endmacro(BuildIgnore)

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

	macro(BuildApplication name)

	if(ANDROID AND NOT PB_FORCE_PROJECT)

		BuildLibrary(${name} SHARED ${SOURCES})

	else()

		BuildTargetBegin(${name})
		add_executable(${name} ${SOURCES})
		BuildTargetEnd(${name})

	endif()

	endmacro(BuildApplication name)

	# Helpers

	macro(BuildTargetBegin name)

		include_directories(${INCLUDES})

	endmacro(BuildTargetBegin name)

	macro(BuildTargetEnd name)

		target_link_libraries(${name} PUBLIC ${LIBS})
		target_compile_definitions(${name} PUBLIC ${DEFINES}) 
		target_compile_definitions(${name} PRIVATE ${DEFINES_PRIVATE}) 

	endmacro(BuildTargetEnd name)
