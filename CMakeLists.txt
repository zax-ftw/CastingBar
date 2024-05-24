cmake_minimum_required(VERSION 3.21)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")

########################################################################################################################
## Define project
########################################################################################################################
project(
    CastingBar
    VERSION 3.0.0
    LANGUAGES CXX
)

set(AUTHOR_NAME "Zax-Ftw")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

set(SOURCE_FILES
 "src/Main.cpp"
 "src/Settings.cpp"
 "src/CastingBar.cpp"
 "src/StateController.cpp" 
 "src/EventHandler.cpp"
 "src/States/DefaultState.cpp" 
 "src/States/RangedState.cpp"
 "src/States/MagicState.cpp" 
 "src/States/RightMagicState.cpp" 
 "src/States/LeftMagicState.cpp" 
 "src/States/VoiceState.cpp" 
 "src/Hooks/PlayerCharacter.cpp")

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Source Files"
    FILES ${SOURCE_FILES}
)

#########################################################################################################################
### Build options
#########################################################################################################################

option(COPY_OUTPUT "Copy the build output to the Skyrim directory." ON)

cmake_host_system_information(
	RESULT SKYRIM_PATH
	QUERY WINDOWS_REGISTRY 
	"HKLM/SOFTWARE/WOW6432Node/Bethesda Softworks/Skyrim Special Edition"
	VALUE "Installed Path"
)

########################################################################################################################
## Configure target DLL
########################################################################################################################

find_package(CommonLibSSE CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

find_path(SIMPLEINI_INCLUDE_DIR "ConvertUTF.c")

add_library(
	${PROJECT_NAME}
	SHARED
	${SOURCE_FILES}
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	.clang-format
	.editorconfig
)

target_commonlibsse_properties(
	${PROJECT_NAME}
	AUTHOR ${AUTHOR_NAME}
	USE_ADDRESS_LIBRARY
)

target_link_libraries(
	${PROJECT_NAME}
    PRIVATE
    CommonLibSSE::CommonLibSSE
	magic_enum::magic_enum
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SIMPLEINI_INCLUDE_DIR}
)

target_precompile_headers(
	${PROJECT_NAME}
    PRIVATE
		src/PCH.h
)

target_compile_features(
	${PROJECT_NAME} 
	PRIVATE 
		cxx_std_23
)

########################################################################################################################
## Automatic plugin deployment
########################################################################################################################

if (COPY_OUTPUT)
	if (DEFINED SKYRIM_PATH)
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${SKYRIM_PATH}/Data/SKSE/Plugins/)		
		if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
			add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${SKYRIM_PATH}/Data/SKSE/Plugins/)
		endif()
	else ()
		message("Installed Path is not defined. Skipping post-build copy command.")
	endif ()
endif ()
