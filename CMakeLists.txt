cmake_minimum_required(VERSION 3.30)

project(tests)
set(PROJ ReverseGUI)

# Cmake Options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global Includes
include_directories(
${CMAKE_SOURCE_DIR}/src
${CMAKE_SOURCE_DIR}/vendor/SDL/include
${CMAKE_SOURCE_DIR}/vendor/raylib/src
${CMAKE_SOURCE_DIR}/vendor/imgui
${CMAKE_SOURCE_DIR}/vendor/imgui/backends
${CMAKE_SOURCE_DIR}/vendor/rlImGui
${CMAKE_SOURCE_DIR}/vendor/rlImGui/examples)

# SDL3 Options
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(SDL_EXAMPLES OFF)

# Build SDL3
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/SDL  EXCLUDE_FROM_ALL)

# Raylib Options
set(BUILD_SHARED_LIBS ON)
set(BUILD_EXAMPLES OFF)
set(PLATFORM SDL)

# Build Raylib
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/raylib)

# rlImGui Options
Set(RLIMGUI_EXTRAS OFF)

# Compile Executables
add_library(imgui STATIC
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp)
	
add_library(rlimgui STATIC
${CMAKE_SOURCE_DIR}/vendor/rlimgui/rlimgui.cpp)

set(proj_sources
    src/main.cpp
    src/exe_path.cpp
    res/icon_info.rc)

if(WIN32 AND NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_executable(${PROJ} WIN32
		${proj_sources})
else()
	add_executable(${PROJ}
		${proj_sources})
endif()
    
# Test Module (Scratch Pad)
add_executable(scratch
	src/scratch.cpp)

target_link_libraries(${PROJ} raylib imgui rlimgui)